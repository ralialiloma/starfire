// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Sf_bases/Sf_CharacterMovementComponent.h"
#include "Sf_FP_CharacterMovementComponent.generated.h"

#pragma region StructsAndEnums
UENUM(BlueprintType)
enum ECustomMovementMode : int
{
	CMOVE_None  UMETA(Hidden),
	CMOVE_WallRun UMETA(DisplayName = "Wall Run"),
	CMOVE_Mantle UMETA(DisplayName = "Mantle"),
	CMOVE_Dash UMETA(DisplayName = "Dash"),
};
#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintChange,bool, Pressed);

DEFINE_LOG_CATEGORY_STATIC(SF_FP_CharacterMovement, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API USf_FP_CharacterMovementComponent : public USf_CharacterMovementComponent
{
	GENERATED_BODY()

#pragma region SavedMove_SF
	class FSavedMove_Sf: public  FSavedMove_Character
	{
	public:
		enum CompressedFlags
		{
			Flag_WallRun = 0x10,
			Flag_Sprint = 0x20,
		};


		//Flags
		uint8 Saved_bWantsToSprint:1;

		//Other Variables
		uint8 Saved_bWallRunIsRight:1;
		uint8 Saved_bCustomJump:1;

		FSavedMove_Sf();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
#pragma endregion

public:
	virtual void InitializeComponent() override;
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode=0) override;
	virtual void Crouch(bool bClientSimulation) override;

#pragma region Functions
public:
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintCallable)
	ECustomMovementMode GetCustomMovementMode() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnSprintChange OnSprintChange;

protected:
	void ReportNoiseEvent(float Loudness);
#pragma endregion
	
#pragma region Properties

public:
	UPROPERTY(Transient)
	ASf_FP_Character* SfCharacterOwner;
	
	UPROPERTY(Transient)
	TEnumAsByte<EMovementMode> PreviousMovementMode;
	UPROPERTY(Transient)
	uint8 PreviousCustomMode;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Jumping / Falling", meta =(CustomConfig))
	float InAirJumpAllowance = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Jumping / Falling", meta =(CustomConfig))
	float Jump_NoiseEventVolume = 0.6f;
	
	UPROPERTY(Transient)
	float ElapsedInAirJumpAllowance = 0.f;
	UPROPERTY(Transient)
	bool bWasWallRunningBeforeAllowance = false;

#pragma endregion
	
#pragma region Sprint
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanSprint();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSprinting();
	
	UFUNCTION(BlueprintCallable)
	void SprintPressed();

	UFUNCTION(BlueprintCallable)
	void SprintReleased();
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Crouch_MaxWalkSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Sprint_MaxWalkSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Walk_MaxWalkSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Walk_NoiseEventVolume = 0.4f;
private:
	bool Safe_bWantsToSprint;
#pragma endregion
	
#pragma endregion
	
#pragma region WallRun
	
#pragma region Functions
public:
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const { return  IsCustomMovementMode (CMOVE_WallRun); }
	UFUNCTION(BlueprintPure)
	bool WallRunningIsRight() const { return  Saved_bWallRunIsRight; }
private:

	float GetVectorAngleFrom(FVector Vector, FVector Control = FVector::UpVector) const;
	float GetVectorAngleOn(FVector Vector, FVector Forward, FVector Plane = FVector::UpVector) const;
	bool CheckWallSteepness(FVector Normal) const;
	FHitResult CheckForWall(FVector2D WallNormal, float ForwardOffset = 0) const;
	FHitResult CheckFromPlayer(const FVector& CastDelta, const FVector& Offset = FVector::Zero()) const;
	
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);
	void AdjustForWallDistance(const FHitResult& WallHit, const FVector& SmoothedWallNormal3D);
	void JumpOffWall();
#pragma endregion
	
#pragma region Properties
protected:

	// Add a member variable to your character movement component's header file, for example:
	FVector2D SmoothedWallNormal = FVector2D::ZeroVector; // This will hold the currently smoothed wall normal.
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig, InlineEditConditionToggle))
	bool HasMinWallRunSpeed = false;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig, EditCondition = "HasMinWallRunSpeed"))
	float MinWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxWallRunSpeed = 800.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxVerticalWallRunSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig, InlineEditConditionToggle))
	bool HasWallPullAwayAngle = false;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig, EditCondition = "HasWallPullAwayAngle"))
	float WallRunPullAwayAngle = 150;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxWallRunSteepnessDeviation = 25.f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	UCurveFloat* WallRunGravityScaleCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun|Jump", meta =(CustomConfig))
	bool JumpTowardsPlayerForward = true;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun|Jump", meta =(CustomConfig, EditConditionHides = "!JumpTowardsPlayerForward"))
	float WallJumpOffForce = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun|Jump", meta =(CustomConfig,ClampMin=0, ClampMax=1, EditConditionHides = "!JumpTowardsPlayerForward"))
	float WallNormalJumpOffInfluence= 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun|Jump", meta =(CustomConfig,ClampMin=0, ClampMax=1, EditConditionHides = "JumpTowardsPlayerForward"))
	float WallJumpForceMultiplier= 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallDistanceToCapsule = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallRun_NoiseEventVolume = 0.4f;

private:
	FVector2D PreviousWallNormal = FVector2D::ZeroVector;
	bool Saved_bWallRunIsRight = true;
#pragma endregion
	
#pragma endregion
	
#pragma region Mantle
	
#pragma region Functions
	//Mantle
	bool CanMantle() const;
	bool TryMantle();
	void PhysMantle(float deltaTime, int32 Iterations);
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	bool bRequireInputForMantle = false;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinDistance = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxDistance = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxHeight = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinWallAngle = 75.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxSurfaceAngle = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleAlignmentAngle = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinVelocity = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	UAnimMontage* MantleMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float Mantle_NoiseEventVolume = 0.8f;
private:
	UPROPERTY()
	FVector MantleTargetLocation;
	UPROPERTY()
	FVector MantleOriginLocation;
	UPROPERTY()
	FVector MantleStartingVelocity;
	UPROPERTY()
	float ElapsedMantleTime = 0;
#pragma endregion

#pragma endregion
	
#pragma region Dash
	
	bool TryDash() const;
	bool PhysDash(float DeltaTime, int32 Iterations);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig, BitMask, BitmaskEnum = EMovementMode))
	int32 DashRechargeStates = (1 << MOVE_NavWalking) | (1 << MOVE_Walking);
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig, BitMask, BitmaskEnum = ECustomMovementMode))
	int32 DashCustomRechargeStates = (1 << CMOVE_WallRun);
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig))
	float MaxDashDuration = 0.3f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig))
	float Dash_MaxWalkSpeed = 1200;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig, ClampMin = 0))
	int MaxDashes = 1;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Dash", meta =(CustomConfig))
	float Dash_NoiseEventVolume = 0.5f;

	UPROPERTY()
	int DashCount = 0;
	UPROPERTY()
	float DashDuration = 0;
	UPROPERTY()
	FVector DashDirection = FVector::ZeroVector;

#pragma endregion
};
