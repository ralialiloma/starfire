// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnDeath);

UCLASS(Abstract)
class STARFIRE_API ASf_CharacterBase : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPawnDeath OnPawnDeath;

	UFUNCTION(BlueprintNativeEvent)
	void PawnDeath();
	
};
