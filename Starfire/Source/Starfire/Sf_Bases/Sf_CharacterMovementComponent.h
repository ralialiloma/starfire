// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Sf_CharacterMovementComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_CharacterMovement, Display, Display);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementModeChanged, EMovementMode, PreviousMovementMode, EMovementMode, NewMovementMode);

UCLASS()
class STARFIRE_API USf_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode=0) override;
#pragma region Functions
public:
	UFUNCTION(BlueprintPure)
	virtual bool IsMovementMode(EMovementMode InMovementMode) const;
protected:
	virtual float CapRadius() const;
	virtual float CapHalfHeight() const;
#pragma endregion

#pragma region Properties
	UPROPERTY(BlueprintAssignable)
	FOnMovementModeChanged OnMovementModeChanged;
#pragma endregion
	
};
	
