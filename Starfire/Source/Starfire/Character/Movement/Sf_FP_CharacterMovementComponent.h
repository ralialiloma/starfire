// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Character/Sf_Character.h"
#include "Starfire/Sf_bases/Sf_CharacterMovementComponent.h"
#include "Sf_FP_CharacterMovementComponent.generated.h"

#pragma region StructsAndEnums
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None  UMETA(Hidden),
	CMOVE_WallRun UMETA(DisplayName = "Wall Run"),
	CMOVE_Mantle UMETA(DisplayName = "Mantle"),
};
#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintChange,bool, Pressed);

DEFINE_LOG_CATEGORY_STATIC(SF_FP_CharacterMovement, Display, Display);

UCLASS()
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
#pragma endregion
	
#pragma region Properties
	UPROPERTY(Transient)
	ASf_Character* SfCharacterOwner;
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
	float Sprint_MaxWalkspeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Walk_MaxWalkSpeed = 200.0f;
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
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MinWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxWallRunSpeed = 800.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxVerticalWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallRunPullAwayAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallAttractionForce = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	UCurveFloat* WallRunGravityScaleCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallJumpOffForce = 300.f;
	
	//Transient
	UPROPERTY(Transient)
	ASf_Character* SfCharacterOwner;

	FVector2D WallNormal = FVector2D::ZeroVector;

	bool Saved_bWallRunIsRight;
#pragma endregion
	
#pragma endregion

	
#pragma region Mantle
	
#pragma region Functions
	//Mantle
	bool TryMantle();
	void PhysMantle(float deltaTime, int32 Iterations);
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinDistance = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxDistance = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxHeight = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleLowerDeviation = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinWallAngle = 75.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxSurfaceAngle = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleAlignmentAngle = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinVelocityForBoost = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMinVelocity = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	float MantleMaxDuration = 0.7f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	UCurveFloat* MantleCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: Mantle", meta =(CustomConfig))
	UAnimMontage* MantleMontage = nullptr;
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
	

};
