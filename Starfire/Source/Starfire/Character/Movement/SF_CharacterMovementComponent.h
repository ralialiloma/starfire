// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Character/Sf_Character.h"
#include "SF_CharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None  UMETA(Hidden),
	CMOVE_WallRun UMETA(DisplayName = "Wall Run"),
};

/**
 * 
 */
UCLASS()
class STARFIRE_API USF_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

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

		//Sprint
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Sprint_MaxWalkspeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Walking", meta =(CustomConfig))
	float Walk_MaxWalkSpeed = 200.0f;
	
		//WallRun
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MinWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxWallRunSpeed = 800.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MaxVerticalWallRunSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallRunPullAwayAngle =75;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallAttractionForce = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	UCurveFloat* WallRunGravityScaleCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMovement: WallRun", meta =(CustomConfig))
	float WallJumpOffForce = 300.f;;
	
	
	//Transient
	UPROPERTY(Transient) ASf_Character* SfCharacterOwner;
	
	bool Safe_bWallRunIsRight;
	bool Safe_bWantsToSprint;

	//Character Movement Component
public:
	virtual void InitializeComponent() override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
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

	UFUNCTION(BlueprintCallable)
	void SprintPressed();

	UFUNCTION(BlueprintCallable)
	void SprintReleased();

private:
	float CapRadius() const;

	float CapHalfHeight() const;



	//Vault
private:
	//WallRun
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);

};
