// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sf_Delegate.h"
#include "Components/BoxComponent.h"
#include "Components/Sf_Equipment.h"
#include "GameFramework/Character.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Starfire/Shared/Weapon/Interfaces/WeaponOwner.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/MeleeInfo.h"
#include "Sf_Character.generated.h"

UCLASS(Abstract)
class STARFIRE_API ASf_Character : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);
	
#pragma region Functions
public:
	UFUNCTION(BlueprintNativeEvent)
	void PawnDeath();

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_Equipment* GetSfEquipmentComponent() const{return SfEquipmentComponent;};
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE USf_DamageController* GetSfDamageController() const {return SfDamageController;};
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnPawnDeath_BP;
	FSf_VoidDelegate_CPP OnPawnDeath_CPP;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	USf_Equipment* SfEquipmentComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	UBoxComponent* MeleeTransform;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageController* SfDamageController;
#pragma endregion

#pragma region IWeaponOwnerInterface
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual FTransform GetFireTransform_Implementation() const override;
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
	virtual USf_AnimDataController* GetAnimMontageController_Implementation() const override;
#pragma endregion



	
};
