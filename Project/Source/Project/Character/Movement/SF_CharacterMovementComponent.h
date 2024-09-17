// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Project/Character/SF_Character.h"
#include "SF_CharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_WallRun UMETA(DisplayName = "Wall Run"),
};

/**
 * 
 */
UCLASS()
class PROJECT_API USF_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Sf: public  FSavedMove_Character
	{
		public:
			enum CompressedFlags
			{
				Flag_WallRun = 0x10,
			};


		//Flags
		//uint8 Saved

		//Other Variables
		uint8 Saved_bWallRunIsRight:1;

		FSavedMove_Sf();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

	};

	class FNetworkPredictionData_Client_Sf: public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Sf(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	//Parameters
	UPROPERTY(EditDefaultsOnly) float MinWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly) float MaxWallRunSpeed = 800.0f;
	UPROPERTY(EditDefaultsOnly) float MaxVerticalWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly) float WallRunPullAwayAngle =75;
	UPROPERTY(EditDefaultsOnly) float WallAttractionForce = 200.0f;
	UPROPERTY(EditDefaultsOnly) float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly) UCurveFloat* WallRunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly) float WallJumpOffForce = 300.f;;
	
	
	//Transient
	UPROPERTY(Transient) ASf_Character* SfCharacterOwner;
	
	bool Safe_bWallRunIsRight;

	//Character Movement Component
public:
	virtual void InitializeComponent() override;
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	//Interface
public:
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;
	
	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const {return  IsCustomMovementMode (CMOVE_WallRun);};
	UFUNCTION(BlueprintPure)
	bool WallRunningIsRight() const {return  Safe_bWallRunIsRight;};

private:
	float CapRadius() const;

	float CapHalfHeight() const;



	//Vault
private:
	//WallRun
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);

};
