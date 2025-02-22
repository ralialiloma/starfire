﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_HitReact.generated.h"

class UCF_HitReact_Config;
/**
 * 
 */
UCLASS()
class STARFIRE_API UCF_HitReact : public USf_CharacterFeature
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(ASf_TP_Character* OwningCharacterIn, const USf_CharacterFeature_Config* InConfig) override;
	
	UFUNCTION()
	void OnDamage(float RemainingHealth, float DamageReceived, FVector HitLocation, FVector HitNormal, FGameplayTag DamageType);
	
	UPROPERTY()
	FName LastHitBone = NAME_None;
	UPROPERTY()
	FVector LastHitDirection = FVector::ZeroVector;
	UPROPERTY()
	FVector LastHitLocation = FVector::ZeroVector;

protected:

	UFUNCTION()
	void PlayMontage(UAnimMontage* Montage);
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY()
	const UCF_HitReact_Config* HitReactConfig;
	UPROPERTY()
	bool bMontageCooldown = false;

};
