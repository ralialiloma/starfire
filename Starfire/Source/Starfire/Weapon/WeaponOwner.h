// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeInfo.h"
#include "Starfire/Animation/WeaponAnimMontageController.h"
#include "UObject/Interface.h"
#include "WeaponOwner.generated.h"



// This class does not need to be modified.
UINTERFACE()
class UWeaponOwner : public UInterface
{
	GENERATED_BODY()
};


class STARFIRE_API IWeaponOwner: public IInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponOwner")
	UAnimInstance* GetCharacterAnimInstance() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponOwner")
	USf_WeaponAnimMontageController* GetAnimMontageController() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponOwner")
	FTransform GetFireTransform() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponOwner")
	FMeleeInfo GetMeleeInfo() const;
};
