// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Sf_bases/Sf_CharacterMovementComponent.h"
#include "Sf_TP_CharacterMovementComponent.generated.h"

class ASf_TP_Character;

DEFINE_LOG_CATEGORY_STATIC(SF_TP_CharacterMovement, Display, Display);
UCLASS()
class STARFIRE_API USf_TP_CharacterMovementComponent : public USf_CharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
#pragma region Functions

#pragma endregion
	
	
#pragma region Properties
protected:
	UPROPERTY(Transient)
	ASf_TP_Character* SfCharacterOwner;
#pragma endregion

	
#pragma region FSavedMove_Sf
	class FSavedMove_Sf: public  FSavedMove_Character
	{
	public:
		enum CompressedFlags
		{
			Flag_Sprint = 0x20,
		};


		//Flags
		uint8 Saved_bWantsToSprint:1;

		FSavedMove_Sf();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
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
	UPROPERTY()
	bool Safe_bWantsToSprint;
#pragma endregion

#pragma region Strafe
	#pragma region Functions
	public:
		UFUNCTION(BlueprintCallable)
		void SetStrafe(const bool bWantsToStrafe);
		UFUNCTION(BlueprintCallable,BlueprintPure)
		bool IsStrafing();
	#pragma endregion
	#pragma region Properties
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Rotation Settings", meta =(CustomConfig))
	FRotator DefaultRotationRate = FRotator(0,0,200);

	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Rotation Settings", meta =(CustomConfig))
	FRotator FallRotationRate = FRotator(0,0,-1);
	protected:
		UPROPERTY(Transient)
		bool Safe_bWantsToStrafe = false;
#pragma endregion
#pragma endregion
};
