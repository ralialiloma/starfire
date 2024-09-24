// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(SF_CharacterMovement, Display, Display);

USF_CharacterMovementComponent::FSavedMove_Sf::FSavedMove_Sf(): Saved_bWantsToSprint(0), Saved_bWallRunIsRight(0)
{
}

bool USF_CharacterMovementComponent::FSavedMove_Sf::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Sf* NewSfMove = static_cast<FSavedMove_Sf*>(NewMove.Get());

	if (Saved_bWallRunIsRight != NewSfMove->Saved_bWallRunIsRight)
		return false;

	if (Saved_bWantsToSprint!= NewSfMove->Saved_bWantsToSprint)
		return false;

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USF_CharacterMovementComponent::FSavedMove_Sf::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWallRunIsRight = 0;
	Saved_bWantsToSprint = 0;
}

uint8 USF_CharacterMovementComponent::FSavedMove_Sf::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();
	
	if (Saved_bWantsToSprint) Result = Flag_Sprint;
	
	return Result;
}

void USF_CharacterMovementComponent::FSavedMove_Sf::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	const USF_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USF_CharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWallRunIsRight = CharacterMovementComponent->Safe_bWallRunIsRight;
	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
}

void USF_CharacterMovementComponent::FSavedMove_Sf::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	USF_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USF_CharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovementComponent->Safe_bWallRunIsRight = Saved_bWallRunIsRight;
	CharacterMovementComponent->Safe_bWantsToSprint =  Saved_bWantsToSprint;
}

USF_CharacterMovementComponent::FNetworkPredictionData_Client_Sf::FNetworkPredictionData_Client_Sf(
	const UCharacterMovementComponent& ClientMovement): Super(ClientMovement)
{
}

FSavedMovePtr USF_CharacterMovementComponent::FNetworkPredictionData_Client_Sf::AllocateNewMove()
{
	return MakeShared<FSavedMove_Sf>();
}


bool USF_CharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning();
}

bool USF_CharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();

	if (!CharacterOwner || !CharacterOwner->CanJump())
		return false;
	
	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector CastDelta = UpdatedComponent->GetRightVector()* CapRadius()*2;
			FVector End = Safe_bWallRunIsRight?Start+CastDelta:Start-CastDelta;
			FCollisionQueryParams Params = SfCharacterOwner->GetIgnoreCharacterParams();
			FHitResult WallHit;
			Safe_bWallRunIsRight  = GetWorld()->LineTraceSingleByProfile(WallHit,Start,End,"BlockAll",Params);
			Velocity += WallHit.Normal * WallJumpOffForce;
		}
		
		return true;
	}
	
	return false;
	
}

float USF_CharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode !=MOVE_Custom) return Super::GetMaxSpeed();
	switch(CustomMovementMode)
	{
		case CMOVE_WallRun:
			return MaxWallRunSpeed;
		default:
				UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
				return -1.f;
	}
}

float USF_CharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode !=MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch(CustomMovementMode)
	{
		case CMOVE_WallRun:
			return 0.f ;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
			return -1.f;
	}
}

void USF_CharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (IsFalling())
	{
		TryWallRun();
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void USF_CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkspeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void USF_CharacterMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	
	switch (CustomMovementMode)
	{
	case CMOVE_WallRun:
			PhysWallRun(DeltaTime,Iterations);
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

bool USF_CharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool USF_CharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return MovementMode == InMovementMode;
}

void USF_CharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SfCharacterOwner = Cast<ASf_Character>(CharacterOwner);
}

FNetworkPredictionData_Client* USF_CharacterMovementComponent::GetPredictionData_Client() const
{
	check (PawnOwner!=nullptr)

	if (ClientPredictionData == nullptr)
	{
		USF_CharacterMovementComponent* MutableThis = const_cast<USF_CharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Sf(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	
	return ClientPredictionData;
}

void USF_CharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Safe_bWantsToSprint = (Flags & FSavedMove_Sf::Flag_Sprint)!=0;
	Super::UpdateFromCompressedFlags(Flags);
}

void USF_CharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void USF_CharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

float USF_CharacterMovementComponent::CapRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float USF_CharacterMovementComponent::CapHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

bool USF_CharacterMovementComponent::TryWallRun()
{
	//Check Falling
	if (!IsFalling()) return false;

	//Check Min 2D Speed
	if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2)) return false;

	//Check For Z Velocity
	if (Velocity.Z <-MaxVerticalWallRunSpeed) return false;
	
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector LeftEnd =	Start - UpdatedComponent->GetRightVector()*CapRadius()*2;
	FVector RightEnd = Start + UpdatedComponent->GetRightVector()*CapRadius()*2;
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	FHitResult Floorhit;

	//Check Player Height
	if (GetWorld()->LineTraceSingleByProfile(Floorhit,Start,Start+FVector::DownVector*(CapHalfHeight()+MinWallRunHeight),"BlockAll",Params))
		return false;

	FHitResult Wallhit;
	//Left Cast
	GetWorld()->LineTraceSingleByProfile(Wallhit,Start,LeftEnd,"BlockAll", Params);
	if (Wallhit.IsValidBlockingHit() && (Velocity | Wallhit.Normal)<0)
		Safe_bWallRunIsRight = false;
	else
	{
		GetWorld()->LineTraceSingleByProfile(Wallhit,Start,RightEnd,"BlockAll", Params);
		if (Wallhit.IsValidBlockingHit() && (Velocity | Wallhit.Normal)<0)
			Safe_bWallRunIsRight = true;
		else
			return false;
	}

	//Check Projected Veloctiy
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity,Wallhit.Normal);
	if (ProjectedVelocity.SizeSquared2D()<pow(MinWallRunSpeed,2)) return false;

	//Passed all conditions
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z,0,MaxWallRunSpeed);
	SetMovementMode(MOVE_Custom,CMOVE_WallRun);
	return true;
}

void USF_CharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{
	if (deltaTime<MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner||
		(!CharacterOwner->Controller
			&& !bRunPhysicsWithNoController
			&&!HasAnimRootMotion()
			&&!CurrentRootMotion.HasOverrideVelocity()
			&& (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	if (!IsValid(WallRunGravityScaleCurve))
	{
		UE_LOG(SF_CharacterMovement, Error, TEXT("Missing WallRunGravityScaleCurve"))
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}


	bJustTeleported = false;
	float RemainingTime = deltaTime;

	while ( (RemainingTime >= MIN_TICK_TIME)
		&& (Iterations < MaxSimulationIterations)
		&& CharacterOwner
		&& (CharacterOwner->Controller
			|| bRunPhysicsWithNoController
			|| (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) )
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(RemainingTime,Iterations);
		RemainingTime-=timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector CastDelta = UpdatedComponent->GetRightVector()*CapRadius()*2;
		FVector End = Safe_bWallRunIsRight?Start+CastDelta:Start-CastDelta;
		auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
		float SinPullAwayangle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));
		FHitResult Wallhit;
		GetWorld()->LineTraceSingleByProfile(Wallhit,Start,End,"BlockAll",Params);
		bool bWantToPullAway = Wallhit.IsValidBlockingHit()
			&& !Acceleration.IsNearlyZero() && (Acceleration.GetSafeNormal()|Wallhit.Normal)>SinPullAwayangle;

		if (!Wallhit.IsValidBlockingHit()||bWantToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(RemainingTime,Iterations);
			return;
		}

		//Clamp Acceleration
		Acceleration = FVector::VectorPlaneProject(Acceleration,Wallhit.Normal);
		Acceleration.Z = 0;
		CalcVelocity(timeTick,0,false,GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity,Wallhit.Normal);
		float TangentAccel = Acceleration.GetSafeNormal()| Velocity.GetSafeNormal2D();
		bool bVelUp = Velocity.Z>0.f;

		Velocity.Z +=GetGravityZ()*WallRunGravityScaleCurve->GetFloatValue(bVelUp?0.f:TangentAccel)*timeTick;

		if (Velocity.SizeSquared2D() < pow (MinWallRunSpeed,2)||Velocity.Z<-MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(RemainingTime,Iterations);
			return;
		}

		//Compute move parameter
		const FVector Delta = timeTick*Velocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		if (bZeroDelta)
		{
			RemainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(),true,Hit);
			FVector WallAttractionDelta = -Wallhit.Normal*WallAttractionForce*timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta,UpdatedComponent->GetComponentQuat(),true,Hit);
		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			RemainingTime = 0.f;
		}
		Velocity = (UpdatedComponent->GetComponentLocation()- OldLocation) /timeTick; //v = dx/dt
	}

	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector CastDelta = UpdatedComponent->GetRightVector()*CapRadius()*2;
	FVector End = Safe_bWallRunIsRight? Start+CastDelta: Start-CastDelta;
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	FHitResult Floorhit, Wallhit;
	GetWorld()->LineTraceSingleByProfile(Wallhit,Start,End,"BlockAll",Params);
	GetWorld()->LineTraceSingleByProfile(Floorhit,Start,Start+FVector::DownVector*(CapHalfHeight()+MinWallRunHeight* .5f),"BlockAll",Params);
	if (Floorhit.IsValidBlockingHit() || !Wallhit.IsValidBlockingHit()||Velocity.SizeSquared2D()<pow(MinWallRunSpeed,2))
	{
		SetMovementMode(MOVE_Falling);
	}
	
}

