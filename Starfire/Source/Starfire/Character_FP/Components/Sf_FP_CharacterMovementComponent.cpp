// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_FP_CharacterMovementComponent.h"

#include "DebugFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

#pragma region HelperMacros
#if 1
#define PRINT(x)  \
	DEBUG_SIMPLE(SF_FP_CharacterMovement, Log, FColor::Yellow, x, Sf_GameplayTags::Debug::FP::Movement::Name)
#define PRINTCOLOR(x,c)  \
	DEBUG_SIMPLE(SF_FP_CharacterMovement, Log, c, FString::Printf(x), Sf_GameplayTags::Debug::FP::Movement::Name)
#define POINT(x, c)  \
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual)) \
		DrawDebugPoint(GetWorld(), x, 10, c, false, UDebugFunctionLibrary::GetDebugDuration(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual));
#define LINE(x1, x2, c) \
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual)) \
		DrawDebugLine(GetWorld(), x1, x2, c, false, UDebugFunctionLibrary::GetDebugDuration(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual));
#define CAPSULE(x, c) \
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual)) \
		DrawDebugCapsule(GetWorld(), x, CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), CharacterOwner->GetCapsuleComponent() \
		->GetScaledCapsuleRadius(), FQuat::Identity, c, false, UDebugFunctionLibrary::GetDebugDuration(Sf_GameplayTags::Debug::FP::Movement::Name, EDebugDisplayType::Visual));
#else
#define PRINT(x)
#define PRINTCOLOR(x,c)
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
	{
		return false;
	}

	if (Saved_bWantsToSprint != NewSfMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USf_FP_CharacterMovementComponent::FSavedMove_Sf::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWallRunIsRight = 0;
	Saved_bWantsToSprint = 0;
}

void USf_FP_CharacterMovementComponent::FSavedMove_Sf::SetMoveFor(ACharacter* C, float InDeltaTime, const FVector& NewAccel,
                                                                  FNetworkPredictionData_Client_Character& ClientData)
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
	CharacterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
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
	{
		return false;
	}

	//Jump FX
	if (IsMovementMode(MOVE_Walking))
	{
		UFXSubsystem::Get(this)->PlayFXOn(Sf_GameplayTags::Effects::Messages::FP::Movement::Jump, GetOwner()->GetRootComponent());
	}

	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning || bWasWallRunningBeforeAllowance)
		{
			JumpOffWall();
			UFXSubsystem::Get(this)->PlayFXOn(Sf_GameplayTags::Effects::Messages::FP::Movement::Jump, GetOwner()->GetRootComponent());
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
		{
			return Crouch_MaxWalkSpeed;
		}

		if (Safe_bWantsToSprint)
		{
			return Sprint_MaxWalkSpeed;
		}

		return Walk_MaxWalkSpeed;
	}

	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_WallRun:
		return MaxWallRunSpeed;
	case CMOVE_Mantle:
		return Safe_bWantsToSprint ? Sprint_MaxWalkSpeed : Walk_MaxWalkSpeed;
	case CMOVE_Dash:
		return Dash_MaxWalkSpeed;
	default:
		UE_LOG(SF_FP_CharacterMovement, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

float USf_FP_CharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxBrakingDeceleration();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_WallRun:
	case CMOVE_Mantle:
	case CMOVE_Dash:
		return 0.f;
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

	if (IsFalling())
	{
		ElapsedInAirJumpAllowance += DeltaSeconds;
	}

	if (!bRequireInputForMantle && (SfCharacterOwner->bCustomJumpDown) && TryMantle())
	{
		SetMovementMode(MOVE_Custom, CMOVE_Mantle);
		ElapsedMantleTime = 0;
		MantleStartingVelocity = Velocity;
		StopMovementImmediately();
	}
	else if (SfCharacterOwner->bCustomJumpPressed)
	{
		if (TryDash())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Dash);
			UFXSubsystem::Get(this)->PlayFXOn(Sf_GameplayTags::Effects::Messages::FP::Movement::Dash, GetOwner()->GetRootComponent());
			DashDuration = MaxDashDuration;
			DashCount++;
			SfCharacterOwner->bCustomJumpPressed = false;
		}
		else if (bRequireInputForMantle && TryMantle())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Mantle);
			ElapsedMantleTime = 0;
			MantleStartingVelocity = Velocity;
			StopMovementImmediately();

			UFXSubsystem::Get(this)->PlayFXOn(Sf_GameplayTags::Effects::Messages::FP::Movement::Mantle, GetOwner()->GetRootComponent());
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

void USf_FP_CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
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
	case CMOVE_Dash:
		PhysDash(DeltaTime, Iterations);
		break;
	default:
		UE_LOG(SF_FP_CharacterMovement, Fatal, TEXT("Invalid Movement Mode"))
	}
}

void USf_FP_CharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (MovementMode != NewMovementMode || CustomMovementMode != NewCustomMode)
	{
		PreviousMovementMode = MovementMode;
		PreviousCustomMode = CustomMovementMode;

		//Dash Recharge
		if ((DashRechargeStates & (1 << NewMovementMode)) != 0 || (DashCustomRechargeStates & (1 << NewCustomMode)) != 0)
		{
			DashCount = 0;

			//Debug Message
			if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::Movement::Dash, EDebugDisplayType::Print))
			{
				FString EnumName = "";
				if (NewMovementMode == MOVE_Custom)
				{
					EnumName = StaticEnum<ECustomMovementMode>()->GetDisplayNameTextByValue(NewCustomMode).ToString();
				}
				else
				{
					EnumName = USf_FunctionLibrary::GetEnumAsString<EMovementMode>(NewMovementMode);
				}

				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Resetting Dash because of new movement mode " + EnumName);
			}
		}

		//In Air Jump Allowance
		if (NewMovementMode == MOVE_Falling &&
			(PreviousMovementMode == MOVE_Walking || PreviousMovementMode == MOVE_NavWalking ||
				CMOVE_WallRun == PreviousCustomMode || CMOVE_Mantle == PreviousCustomMode))
		{
			ElapsedInAirJumpAllowance = 0;
			bWasWallRunningBeforeAllowance = (PreviousMovementMode == MOVE_Custom && (PreviousCustomMode == CMOVE_WallRun));
		}
		else
		{
			ElapsedInAirJumpAllowance += InAirJumpAllowance;
		}

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
	{
		return;
	}

	Safe_bWantsToSprint = true;
	OnSprintChange.Broadcast(true);
}

void USf_FP_CharacterMovementComponent::SprintReleased()
{
	if (!Safe_bWantsToSprint)
	{
		return;
	}

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

float USf_FP_CharacterMovementComponent::GetVectorAngleFrom(FVector Vector, FVector Control) const
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Vector, Control)));
}

float USf_FP_CharacterMovementComponent::GetVectorAngleOn(FVector Vector, FVector Forward, FVector Plane) const
{
	FVector ForwardOnPlane = FVector::VectorPlaneProject(Forward, Plane).GetSafeNormal();
	FVector VectorOnPlane = FVector::VectorPlaneProject(Vector, Plane).GetSafeNormal();
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(VectorOnPlane, ForwardOnPlane)));
}

bool USf_FP_CharacterMovementComponent::CheckWallSteepness(FVector Normal) const
{
	float WallSteepnessAngleDeviation = GetVectorAngleFrom(Normal, FVector::UpVector) - 90.f;
	return FMath::Abs(WallSteepnessAngleDeviation) < MaxWallRunSteepnessDeviation;
}

FHitResult USf_FP_CharacterMovementComponent::CheckForWall(FVector2D WallNormal, float ForwardOffset) const
{
	FVector CastDelta = -FVector(WallNormal.X, WallNormal.Y, 0) * CapRadius() * 2;
	FVector2D WallForward2D = WallNormal.GetRotated(Saved_bWallRunIsRight ? 90 : -90);
	FVector WallForward = FVector(WallForward2D.X, WallForward2D.Y, 0).GetSafeNormal();
	return CheckFromPlayer(CastDelta, (WallForward * ForwardOffset));
}

FHitResult USf_FP_CharacterMovementComponent::CheckFromPlayer(const FVector& CastDelta, const FVector& Offset) const
{
	FVector Start = UpdatedComponent->GetComponentLocation() + Offset;
	FVector End = Start + CastDelta;
	auto Params = SfCharacterOwner->GetIgnoreCharacterParams();
	FHitResult WallHit;
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "MovementTrace", Params);
	return WallHit;
}

bool USf_FP_CharacterMovementComponent::TryWallRun()
{
	//Check Falling
	if (!IsFalling())
	{
		return false;
	}

	//Check Min 2D Speed
	if (HasMinWallRunSpeed && Velocity.SizeSquared2D() < FMath::Square(MinWallRunSpeed))
	{
		return false;
	}

	//Check Player Height - ensure we are not too close to the ground
	const FHitResult FloorHit = CheckFromPlayer(FVector::DownVector * (CapHalfHeight() + MinWallRunHeight));
	if (FloorHit.IsValidBlockingHit())
	{
		return false;
	}

	// Directions to check
	const FVector PlayerForward = UpdatedComponent->GetForwardVector();
	const FVector PlayerRight = UpdatedComponent->GetRightVector();
	const float TraceDist = CapRadius() * 2.0f;

	// We'll store our candidate wall hit here
	FHitResult WallHit;
	bool bFoundWall = false;
	bool bIsRightSide = false;

	auto IsWallRunnable = [&](const FHitResult& Hit) -> bool
	{
		return Hit.IsValidBlockingHit()
			&& CheckWallSteepness(Hit.Normal)
			&& ((Velocity | Hit.Normal) < 0);
	};

	// Forward check
	FHitResult ForwardHit = CheckFromPlayer(PlayerForward * TraceDist);
	if (IsWallRunnable(ForwardHit))
	{
		WallHit = ForwardHit;
		bFoundWall = true;

		float DotRight = FVector::DotProduct(WallHit.Normal.GetSafeNormal2D(), PlayerRight);
		bIsRightSide = DotRight < 0.0f;
	}

	// Left check
	if (!bFoundWall)
	{
		FHitResult LeftHit = CheckFromPlayer(-PlayerRight * TraceDist);
		if (IsWallRunnable(LeftHit))
		{
			WallHit = LeftHit;
			bFoundWall = true;
			bIsRightSide = false;
		}
	}

	// Right check
	if (!bFoundWall)
	{
		FHitResult RightHit = CheckFromPlayer(PlayerRight * TraceDist);
		if (IsWallRunnable(RightHit))
		{
			WallHit = RightHit;
			bFoundWall = true;
			bIsRightSide = true;
		}
	}

	if (!bFoundWall)
	{
		return false;
	}

	Saved_bWallRunIsRight = bIsRightSide;
	PreviousWallNormal = FVector2D(WallHit.Normal.GetSafeNormal2D());

	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);

	if (HasMinWallRunSpeed && ProjectedVelocity.SizeSquared2D() < FMath::Square(MinWallRunSpeed))
	{
		return false;
	}

	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.0f, MaxVerticalWallRunSpeed);
	SmoothedWallNormal = FVector2D::Zero();
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);

	UFXSubsystem::Get(this)->PlayFXOn(Sf_GameplayTags::Effects::Messages::FP::Movement::WallRun::Start, GetOwner()->GetRootComponent());

	return true;
}

#define EXIT_WALLRUNPHYS(ReasonString) \
	SetMovementMode(MOVE_Falling); \
	StartNewPhysics(RemainingTime, Iterations); \
	PRINTCOLOR(TEXT(ReasonString), FColor::Red) \
	return;

void USf_FP_CharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner ||
		(!CharacterOwner->Controller
			&& !bRunPhysicsWithNoController
			&& !HasAnimRootMotion()
			&& !CurrentRootMotion.HasOverrideVelocity()
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
		const float IterationTime = GetSimulationTimeStep(RemainingTime, Iterations);
		RemainingTime -= IterationTime;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		FHitResult WallHit = CheckForWall(PreviousWallNormal);

		FVector2D RawWallNormal2D = FVector2D(WallHit.Normal.GetSafeNormal2D());
		if (SmoothedWallNormal.IsNearlyZero())
		{
			SmoothedWallNormal = RawWallNormal2D;
		}
		else
		{
			SmoothedWallNormal = FMath::Lerp(SmoothedWallNormal, RawWallNormal2D, 0.4);
			SmoothedWallNormal.Normalize();
		}

		if (!CheckWallSteepness(WallHit.Normal))
		{
			EXIT_WALLRUNPHYS("Too Steep");
		}

		// Use the smoothed normal for direction calculations
		FVector2D WallForward2D = SmoothedWallNormal.GetRotated(Saved_bWallRunIsRight ? 90.f : -90.f);
		float PlayerAngleToForward = GetVectorAngleOn(UpdatedComponent->GetForwardVector(), FVector(WallForward2D, 0));
		bool AngleExceedsLimit = HasWallPullAwayAngle && (FMath::Abs(PlayerAngleToForward) > WallRunPullAwayAngle);

		bool bWantToPullAway = !WallHit.IsValidBlockingHit() || Acceleration.IsNearlyZero() || AngleExceedsLimit;
		if (bWantToPullAway)
		{
			EXIT_WALLRUNPHYS("Wants to pull away.");
		}

		float InputMagnitude = Acceleration.Length();
		FVector2D Acceleration2D = WallForward2D * InputMagnitude;
		Acceleration = FVector(Acceleration2D, 0);

		CalcVelocity(IterationTime, 0, false, GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);

		float TangentAccel = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal2D();
		bool bVelUp = Velocity.Z > 0.f;
		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelUp ? 0.f : TangentAccel) * IterationTime;

		if (HasMinWallRunSpeed)
		{
			if (Velocity.SizeSquared2D() < FMath::Square(MinWallRunSpeed) || Velocity.Z < -MaxVerticalWallRunSpeed)
			{
				EXIT_WALLRUNPHYS("WallRun too Slow");
			}
		}

		// Compute move parameter
		const FVector Delta = IterationTime * Velocity;
		if (Delta.IsNearlyZero())
		{
			RemainingTime = 0.f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

			AdjustForWallDistance(WallHit, FVector(SmoothedWallNormal, 0.f));
		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			RemainingTime = 0.f;
		}

		LINE(OldLocation, UpdatedComponent->GetComponentLocation(), FColor::Orange)

		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / IterationTime; // v = dx/dt
		PreviousWallNormal = SmoothedWallNormal; // Store the smoothed normal as the "previous" for next iteration
	}

	FHitResult FinalWallHit = CheckForWall(PreviousWallNormal);
	FHitResult FloorHit = CheckFromPlayer(FVector::DownVector * (CapHalfHeight() + MinWallRunHeight * 0.5f));
	if (FloorHit.IsValidBlockingHit())
	{
		EXIT_WALLRUNPHYS("Floor Found.");
	}
	if (!FinalWallHit.IsValidBlockingHit())
	{
		EXIT_WALLRUNPHYS("No more wall.");
	}
	if (HasMinWallRunSpeed && (Velocity.SizeSquared2D() < FMath::Square(MinWallRunSpeed)))
	{
		EXIT_WALLRUNPHYS("To Slow.");
	}
}


void USf_FP_CharacterMovementComponent::AdjustForWallDistance(const FHitResult& WallHit, const FVector& SmoothedWallNormal3D)
{
	if (WallHit.IsValidBlockingHit())
	{
		float CapsuleRadius, CapsuleHalfHeight;
		CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

		const float DesiredDistance = CapsuleRadius + WallDistanceToCapsule;

		// Calculate perpendicular distance using the smoothed normal rather than the raw one:
		FVector FromWallToPlayer = UpdatedComponent->GetComponentLocation() - WallHit.Location;
		float CurrentPerpDist = FVector::DotProduct(FromWallToPlayer, SmoothedWallNormal3D);

		float DistanceError = DesiredDistance - CurrentPerpDist;

		if (!FMath::IsNearlyZero(DistanceError, KINDA_SMALL_NUMBER))
		{
			const FVector CorrectionDelta = SmoothedWallNormal3D * DistanceError;
			FHitResult Hit;
			SafeMoveUpdatedComponent(CorrectionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);
			LINE(Hit.TraceStart, Hit.TraceEnd, FColor::Red);
			POINT(Hit.TraceStart, FColor::Green);
		}
	}
}

void USf_FP_CharacterMovementComponent::JumpOffWall()
{
	//Input
	FVector InputDir = GetLastInputVector();
	InputDir = FVector(InputDir.X, InputDir.Y, 0);

	if (JumpTowardsPlayerForward)
	{
		Velocity = Velocity.RotateAngleAxis(GetVectorAngleOn(InputDir, Velocity) * (Saved_bWallRunIsRight ? -1 : 1), FVector::UpVector) *
			WallJumpForceMultiplier;
		if (SHOULD_DEBUG(Sf_GameplayTags::Debug::FP::Movement::Wallrun, EDebugDisplayType::Visual) || true)
		{
			UKismetSystemLibrary::DrawDebugArrow(
				this,
				GetActorLocation(),
				GetActorLocation() + Velocity.GetSafeNormal() * 100,
				100,
				FColor::Red,
				10,
				2);
		}
	}
	else
	{
		//Wall
		FHitResult WallHit = CheckForWall(PreviousWallNormal);

		FVector WallJumpOffVector = FMath::Lerp(InputDir, WallHit.Normal, WallNormalJumpOffInfluence).GetSafeNormal();
		Velocity += WallJumpOffVector * WallJumpOffForce;
		if (SHOULD_DEBUG(Sf_GameplayTags::Debug::FP::Movement::Wallrun, EDebugDisplayType::Visual) || true)
		{
			UKismetSystemLibrary::DrawDebugArrow(
				this,
				GetActorLocation(),
				GetActorLocation() + WallJumpOffVector * 100,
				100,
				FColor::Red,
				10,
				2);
		}
	}
}

bool USf_FP_CharacterMovementComponent::CanMantle() const
{
	return (/*IsMovementMode(MOVE_Walking) && !IsCrouching() || */IsMovementMode(MOVE_Falling) || IsCustomMovementMode(CMOVE_Dash));
}

bool USf_FP_CharacterMovementComponent::TryMantle()
{
	if (!CanMantle())
	{
		return false;
	}

	if (Velocity.Length() < MantleMinVelocity)
	{
		PRINT(TEXT("To slow for Mantle."));
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
		// LINE(FrontStart, FrontStart + MantleForward * CheckDistance, FColor::Red)
		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + MantleForward * CheckDistance, "MovementTrace", Params))
		{
			break;
		}
		FrontStart += FVector::UpVector * (2.f * CapHalfHeight() - AdditiveBottomOffset) / 5;
	}
	if (!FrontHit.IsValidBlockingHit())
	{
		return false;
	}

	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	if (FMath::Abs(CosWallSteepnessAngle) > CosMinWallAngle || (MantleForward | -FrontHit.Normal) < CosAlignmentAngle)
	{
		return false;
	}

	// POINT(FrontHit.Location, FColor::Red);

	// Check Top
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit;
	FVector WallUp = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();
	float WallCos = FVector::UpVector | FrontHit.Normal;
	float WallSin = FMath::Sqrt(1 - WallCos * WallCos);
	FVector TraceStart = FrontHit.Location + MantleForward + WallUp * (MantleMaxHeight - AdditiveBottomOffset) / WallSin;
	// LINE(TraceStart, FrontHit.Location + MantleForward, FColor::Orange)
	if (!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + MantleForward, "BlockAll", Params))
	{
		return false;
	}
	for (const FHitResult& Hit : HeightHits)
	{
		if (Hit.IsValidBlockingHit())
		{
			SurfaceHit = Hit;
			break;
		}
	}

	if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMaxSurfaceAngle)
	{
		return false;
	}
	float Height = (SurfaceHit.Location - MantleGroundLocation) | FVector::UpVector;

	// PRINT(FString::Printf(TEXT("Height: %f"), Height))
	// POINT(SurfaceHit.Location, FColor::Blue);

	if (Height > MantleMaxHeight)
	{
		return false;
	}

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
	CAPSULE(MantleTargetLocation, FColor::Green)

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

	if (ElapsedMantleTime > Duration/* * 0.9f*/)
	{
		float MantleBoostDirectionBias = 1.f;
		float MantleBoostVelocity = GetMaxSpeed() * GetLastInputVector().GetSafeNormal().Length();
		Velocity = (GetPawnOwner()->GetControlRotation().Vector() + Direction.GetSafeNormal() * MantleBoostDirectionBias).GetSafeNormal() *
			MantleBoostVelocity;
		// CAPSULE(GetActorLocation(),FColor::Blue)

		SetMovementMode(MOVE_Falling);
	}

	ElapsedMantleTime += deltaTime;
}

bool USf_FP_CharacterMovementComponent::TryDash() const
{
	if (MovementMode != MOVE_Falling)
	{
		return false;
	}

	if (ElapsedInAirJumpAllowance <= InAirJumpAllowance && CharacterOwner->JumpCurrentCount < CharacterOwner->JumpMaxCount)
	{
		return false;
	}

	return DashCount < MaxDashes;
}

bool USf_FP_CharacterMovementComponent::PhysDash(float DeltaTime, int32 Iterations)
{
	if (DeltaTime <= 0.0f)
	{
		SetMovementMode(MOVE_Falling);
		return false;
	}

	DashDuration -= DeltaTime;
	if (DashDuration <= 0.0f)
	{
		SetMovementMode(MOVE_Falling);
		Velocity = Velocity.GetSafeNormal() * GetMaxSpeed();
		return false;
	}

	FVector DashDirection = GetLastInputVector();
	if (DashDirection.IsNearlyZero())
	{
		DashDirection = UpdatedComponent->GetForwardVector();
	}

	DashDirection.Normalize();

	float DashAlpha = DashDuration / MaxDashDuration;
	float CurrentDashSpeed = FMath::Lerp(Sprint_MaxWalkSpeed, Dash_MaxWalkSpeed, DashAlpha);

	Velocity = DashDirection * CurrentDashSpeed;

	FVector DesiredMovement = Velocity * DeltaTime;
	FHitResult Hit;
	SafeMoveUpdatedComponent(DesiredMovement, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.IsValidBlockingHit())
	{
		HandleImpact(Hit, DeltaTime, DesiredMovement);
		SlideAlongSurface(DesiredMovement, 1.0f - Hit.Time, Hit.Normal, Hit, false);
	}

	return true;
}
