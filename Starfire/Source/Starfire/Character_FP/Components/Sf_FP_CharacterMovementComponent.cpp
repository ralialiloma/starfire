// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_FP_CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Starfire/Utility/Debug/DebugSubsystem.h"

#pragma region HelperMacros
#if 1
float MacroDuration = 20.f;
#define PRINT(x)  if (UDebugSubsystem::GetMovementDebug(EDebugType::Print)) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.f, FColor::Yellow, x); if (UDebugSubsystem::GetMovementDebug(EDebugType::Log)) UE_LOG(SF_FP_CharacterMovement, Log, TEXT("%s"), *x);
#define POINT(x, c) if (UDebugSubsystem::GetMovementDebug(EDebugType::Visual)) DrawDebugPoint(GetWorld(), x, 10, c, !MacroDuration, MacroDuration);
#define LINE(x1, x2, c) if (UDebugSubsystem::GetMovementDebug(EDebugType::Visual)) DrawDebugLine(GetWorld(), x1, x2, c, !MacroDuration, MacroDuration);
#define CAPSULE(x, c) if (UDebugSubsystem::GetMovementDebug(EDebugType::Visual)) DrawDebugCapsule(GetWorld(), x, CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), FQuat::Identity, c, !MacroDuration, MacroDuration);
#else
#define SLOG(x)
#define POINT(x, c)
#define LINE(x1, x2, c)
#define CAPSULE(x, c)
#endif
#pragma endregion

USf_FP_CharacterMovementComponent::FSavedMove_Sf::FSavedMove_Sf(): Saved_bWantsToSprint(0), Saved_bWallRunIsRight(0), Saved_bCustomJump(0)
{
}

bool USf_FP_CharacterMovementComponent::FSavedMove_Sf::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Sf* NewSfMove = static_cast<FSavedMove_Sf*>(NewMove.Get());

	if (Saved_bWallRunIsRight != NewSfMove->Saved_bWallRunIsRight)
		return false;

	if (Saved_bWantsToSprint!= NewSfMove->Saved_bWantsToSprint)
		return false;

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USf_FP_CharacterMovementComponent::FSavedMove_Sf::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWallRunIsRight = 0;
	Saved_bWantsToSprint = 0;
}

void USf_FP_CharacterMovementComponent::FSavedMove_Sf::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	const USf_FP_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USf_FP_CharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWallRunIsRight = CharacterMovementComponent->Saved_bWallRunIsRight;
	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
	Saved_bCustomJump = CharacterMovementComponent->SfCharacterOwner->bCustomJumpPressed;
}

void USf_FP_CharacterMovementComponent::FSavedMove_Sf::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	USf_FP_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USf_FP_CharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovementComponent->Saved_bWallRunIsRight = Saved_bWallRunIsRight;
	CharacterMovementComponent->Safe_bWantsToSprint =  Saved_bWantsToSprint;
	CharacterMovementComponent->SfCharacterOwner->bCustomJumpPressed = Saved_bCustomJump;
}

bool USf_FP_CharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning();
}

bool USf_FP_CharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();

	if (!CharacterOwner || !CharacterOwner->CanJump())
		return false;
	
	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector CastDelta = FVector(WallNormal.X, WallNormal.Y, 0) * CapRadius() * 2;
			FVector End = Start - CastDelta;
			FCollisionQueryParams Params = SfCharacterOwner->GetIgnoreCharacterParams();
			FHitResult WallHit;
			Saved_bWallRunIsRight = GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);

			//TODO: Make player jump in input direction
			Velocity += WallHit.Normal * WallJumpOffForce;
		}
		
		return true;
	}
	
	return false;
}

float USf_FP_CharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking)
	{
		if (IsCrouching())
			return Crouch_MaxWalkSpeed;
		
		if (Safe_bWantsToSprint)
			return Sprint_MaxWalkSpeed;

		return Walk_MaxWalkSpeed;
	}
	
	if (MovementMode != MOVE_Custom)
		return Super::GetMaxSpeed();
	
	switch(CustomMovementMode)
	{
		case CMOVE_WallRun:
			return MaxWallRunSpeed;
		case CMOVE_Mantle:
			return Safe_bWantsToSprint ? Sprint_MaxWalkSpeed : Walk_MaxWalkSpeed; 
		default:
				UE_LOG(SF_FP_CharacterMovement, Fatal, TEXT("Invalid Movement Mode"))
				return -1.f;
	}
}

float USf_FP_CharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode !=MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch(CustomMovementMode)
	{
		case CMOVE_WallRun:
			return 0.f ;
		default:
			UE_LOG(SF_FP_CharacterMovement, Fatal, TEXT("Invalid Movement Mode"))
			return -1.f;
	}
}

void USf_FP_CharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (IsFalling())
	{
		TryWallRun();
	}

	if (SfCharacterOwner->bCustomJumpPressed)
	{
		if (TryMantle())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Mantle);
			ElapsedMantleTime = 0;
			MantleStartingVelocity = Velocity;
			StopMovementImmediately();
			CharacterOwner->StopJumping();
		}
		else
		{
			SfCharacterOwner->bCustomJumpPressed = false;
			CharacterOwner->bPressedJump = true;
			CharacterOwner->CheckJumpInput(DeltaSeconds);
		}
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void USf_FP_CharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	//TODO: Maybe fill out
}

void USf_FP_CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void USf_FP_CharacterMovementComponent::PhysCustom(const float DeltaTime, const int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	
	switch (CustomMovementMode)
	{
		case CMOVE_WallRun:
			PhysWallRun(DeltaTime, Iterations);
			break;
		case CMOVE_Mantle:
			PhysMantle(DeltaTime, Iterations);
			break;
		default:
			UE_LOG(SF_FP_CharacterMovement, Fatal, TEXT("Invalid Movement Mode"))
	}
}

void USf_FP_CharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (MovementMode != NewMovementMode || CustomMovementMode != NewCustomMode)
	{
		EMovementMode PreviousMovementMode = MovementMode;

		Super::SetMovementMode(NewMovementMode, NewCustomMode);
		OnMovementModeChanged.Broadcast(PreviousMovementMode, MovementMode);
	}
	Super::SetMovementMode(NewMovementMode, NewCustomMode);
}


bool USf_FP_CharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

void USf_FP_CharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SfCharacterOwner = Cast<ASf_FP_Character>(CharacterOwner);
}

bool USf_FP_CharacterMovementComponent::CanSprint()
{
	return IsMovementMode(MOVE_Walking) && !IsFalling() && !IsCustomMovementMode(CMOVE_WallRun);
}

bool USf_FP_CharacterMovementComponent::IsSprinting()
{
	return Safe_bWantsToSprint;
}

void USf_FP_CharacterMovementComponent::SprintPressed()
{
	if (!CanSprint())
		return;
	
	Safe_bWantsToSprint = true;
	OnSprintChange.Broadcast(true);
}

void USf_FP_CharacterMovementComponent::SprintReleased()
{
	if(!Safe_bWantsToSprint)
		return;
	
	Safe_bWantsToSprint = false;
	OnSprintChange.Broadcast(false);
}

ECustomMovementMode USf_FP_CharacterMovementComponent::GetCustomMovementMode() const
{
	return static_cast<ECustomMovementMode>(CustomMovementMode);
}

void USf_FP_CharacterMovementComponent::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

bool USf_FP_CharacterMovementComponent::TryWallRun()
{
	//Check Falling
	if (!IsFalling())
		return false;

	//Check Min 2D Speed
	if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2))
		return false;

	//Check For Z Velocity
	//if (Velocity.Z <-MaxVerticalWallRunSpeed)
		//return false;
	
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector LeftEnd =	Start - UpdatedComponent->GetRightVector()*CapRadius()*2;
	FVector RightEnd = Start + UpdatedComponent->GetRightVector()*CapRadius()*2;
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	FHitResult Floorhit;

	//Check Player Height
	if (GetWorld()->LineTraceSingleByProfile(Floorhit, Start, Start+FVector::DownVector*(CapHalfHeight()+MinWallRunHeight), "BlockAll", Params))
		return false;

	FHitResult WallHit;
	//Left Cast
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, LeftEnd,"BlockAll", Params);
	if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
	{
		WallNormal = static_cast<FVector2D>(WallHit.Normal.GetSafeNormal2D());
		Saved_bWallRunIsRight = false;
	}
	else
	{
		//Right Cast
		GetWorld()->LineTraceSingleByProfile(WallHit,Start,RightEnd,"BlockAll", Params);
		if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
		{
			WallNormal = static_cast<FVector2D>(WallHit.Normal.GetSafeNormal2D());
			Saved_bWallRunIsRight = true;
		}
		else
		{
			return false;
		}
	}

	//Check Projected Veloctiy
	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
	if (ProjectedVelocity.SizeSquared2D() < pow(MinWallRunSpeed, 2)) return false;

	//Passed all conditions
	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0, MaxWallRunSpeed);
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);
	return true;
}

void USf_FP_CharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
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
		UE_LOG(SF_FP_CharacterMovement, Error, TEXT("Missing WallRunGravityScaleCurve"))
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}


	bJustTeleported = false;
	float RemainingTime = deltaTime;

	while ((RemainingTime >= MIN_TICK_TIME)
		&& (Iterations < MaxSimulationIterations)
		&& CharacterOwner
		&& (CharacterOwner->Controller
			|| bRunPhysicsWithNoController
			|| (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(RemainingTime, Iterations);
		RemainingTime -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector CastDelta = FVector(WallNormal.X, WallNormal.Y, 0) * CapRadius() * 2;
		FVector End = Start - CastDelta;
		auto Params = SfCharacterOwner->GetIgnoreCharacterParams();

		FVector2D WallForward2D = WallNormal.GetRotated(Saved_bWallRunIsRight ? 90 : -90);
		FVector2D PlayerForward2D = FVector2D(UpdatedComponent->GetForwardVector().GetSafeNormal2D());
		float CosineAngle = FVector2D::DotProduct(PlayerForward2D.GetSafeNormal(), WallForward2D.GetSafeNormal());
		float PlayerAngleToForward = FMath::RadiansToDegrees(FMath::Acos(CosineAngle));
		
		FHitResult WallHit;
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
		
		bool bWantToPullAway = !WallHit.IsValidBlockingHit() || Acceleration.IsNearlyZero() || FMath::Abs(PlayerAngleToForward) > WallRunPullAwayAngle;
		if (bWantToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(RemainingTime, Iterations);
			return;
		}

		float InputMagnitude = Acceleration.Length();
		FVector2D Acceleration2D = WallNormal.GetRotated(Saved_bWallRunIsRight ? 90 : -90).GetSafeNormal() * InputMagnitude;
		Acceleration = FVector(Acceleration2D.X, Acceleration2D.Y, 0);
		
		CalcVelocity(timeTick, 0, false, GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
		
		float TangentAccel = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal2D();
		bool bVelUp = Velocity.Z > 0.f;
		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAccel) * timeTick;

		if (Velocity.SizeSquared2D() < pow(MinWallRunSpeed,2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(RemainingTime, Iterations);
			return;
		}

		//Compute move parameter
		const FVector Delta = timeTick * Velocity;
		if (Delta.IsNearlyZero())
		{
			RemainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(),true,Hit);
			FVector WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta,UpdatedComponent->GetComponentQuat(),true,Hit);
		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			RemainingTime = 0.f;
		}
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; //v = dx/dt
	}

	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector CastDelta = FVector(WallNormal.X, WallNormal.Y, 0) * CapRadius() * 2;
	FVector End = Start - CastDelta;
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	FHitResult FloorHit, WallHit;
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", Params);
	GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector * (CapHalfHeight() + MinWallRunHeight * .5f), "BlockAll", Params);
	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(MinWallRunSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
	
}

bool USf_FP_CharacterMovementComponent::TryMantle()
{
	if (!(IsMovementMode(MOVE_Walking) && !IsCrouching() || IsMovementMode(MOVE_Falling)))
	{
		PRINT("Can't Mantle.");
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(Velocity.Length()));
	if (Velocity.Length() < MantleMinVelocity)
	{
		PRINT("To slow for Mantle.");
		return false;
	}

	MantleOriginLocation = UpdatedComponent->GetComponentLocation();
	FVector MantleGroundLocation = MantleOriginLocation - FVector::UpVector * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector MantleForward = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	float CosMinWallAngle = FMath::Cos(FMath::DegreesToRadians(MantleMinWallAngle));
	float CosMaxSurfaceAngle = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
	float CosAlignmentAngle = FMath::Cos(FMath::DegreesToRadians(MantleAlignmentAngle));

	//Check Front
	FHitResult FrontHit;
	float CheckDistance = FMath::Clamp(Velocity | MantleForward, MantleMinDistance, MantleMaxDistance);
	float AdditiveBottomOffset = IsMovingOnGround() ? (MaxStepHeight - 1) : 0;
	FVector FrontStart = MantleGroundLocation + FVector::UpVector * AdditiveBottomOffset;
	for (int i = 0; i < 6; i++)
	{
		LINE(FrontStart, FrontStart + MantleForward * CheckDistance, FColor::Red)
		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + MantleForward * CheckDistance, "BlockAll", Params))
			break;
		FrontStart += FVector::UpVector * (2.f * CapHalfHeight() - AdditiveBottomOffset) / 5;
	}
	if (!FrontHit.IsValidBlockingHit())
		return false;
	
	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	if (FMath::Abs(CosWallSteepnessAngle) > CosMinWallAngle || (MantleForward | -FrontHit.Normal) < CosAlignmentAngle)
		return false;

	POINT(FrontHit.Location, FColor::Red);
	
	// Check Top
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit;
	FVector WallUp = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();
	float WallCos = FVector::UpVector | FrontHit.Normal;
	float WallSin = FMath::Sqrt(1 - WallCos * WallCos);
	FVector TraceStart = FrontHit.Location + MantleForward + WallUp * (MantleMaxHeight - AdditiveBottomOffset) / WallSin;
	LINE(TraceStart, FrontHit.Location + MantleForward, FColor::Orange)
	if (!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + MantleForward, "BlockAll", Params))
		return false;
	for (const FHitResult& Hit : HeightHits)
	{
		if (Hit.IsValidBlockingHit())
		{
			SurfaceHit = Hit;
			break;
		}
	}
	
	if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMaxSurfaceAngle)
		return false;
	float Height = (SurfaceHit.Location - MantleGroundLocation) | FVector::UpVector;

	PRINT(FString::Printf(TEXT("Height: %f"), Height))
	POINT(SurfaceHit.Location, FColor::Blue);
	
	if (Height > MantleMaxHeight)
		return false;

	// Check Clearance
	float SurfaceCos = FVector::UpVector | SurfaceHit.Normal;
	float SurfaceSin = FMath::Sqrt(1 - SurfaceCos * SurfaceCos);
	MantleTargetLocation =
		SurfaceHit.Location
		// + MantleForward * CapRadius()
		+ FVector::UpVector * (CapHalfHeight() + 1 + CapRadius() * 2 * SurfaceSin);
	FCollisionShape CapShape = FCollisionShape::MakeCapsule(CapRadius(), CapHalfHeight());
	if (GetWorld()->OverlapAnyTestByProfile(MantleTargetLocation, FQuat::Identity, "BlockAll", CapShape, Params))
	{
		CAPSULE(MantleTargetLocation, FColor::Red)
		return false;
	}
	else
	{
		CAPSULE(MantleTargetLocation, FColor::Green)
	}
	
	CharacterOwner->PlayAnimMontage(MantleMontage);
	return true;
}

void USf_FP_CharacterMovementComponent::PhysMantle(float deltaTime, int32 Iterations)
{
	const FVector Direction = (MantleTargetLocation - MantleOriginLocation);
	const FVector Movement = MantleStartingVelocity.ProjectOnTo(Direction);
	const float Duration = FMath::Min(Direction.Length() / Sprint_MaxWalkSpeed, MantleMaxDuration);
	const float Alpha = ElapsedMantleTime / Duration;

	CharacterOwner->SetActorLocation(FMath::Lerp(MantleOriginLocation, MantleTargetLocation, Alpha));

	if (ElapsedMantleTime > Duration * 0.9f)
	{
		float MantleBoostDirectionBias = 1.f;
		float MantleBoostVelocity = GetMaxSpeed() * GetLastInputVector().GetSafeNormal().Length();
		Velocity = (GetPawnOwner()->GetControlRotation().Vector() + Direction.GetSafeNormal() * MantleBoostDirectionBias).GetSafeNormal() * MantleBoostVelocity;
		CAPSULE(GetActorLocation(),FColor::Blue)
		
		SetMovementMode(MOVE_Falling);
	}
	
	ElapsedMantleTime += deltaTime;
}
