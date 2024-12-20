﻿#include "WeaponAnimMontageController_TP.h"

#include "Starfire/Animation/AnimationData/AnimDataHelper.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"


void USf_WeaponAnimMontageController_TP::InitializeComponent()
{
	Super::InitializeComponent();
}

#if WITH_EDITOR
void USf_WeaponAnimMontageController_TP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationEventType,FGameplayTag>(AnimationStartMappings);
}
#endif

float USf_WeaponAnimMontageController_TP::RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData)
{
	const EWeaponAnimationEventType EventType = WeaponAnimationUpdateData.AnimationEventType;
	const FGameplayTag* MontageType  =  AnimationStartMappings.Find(EventType);
	if (MontageType == nullptr)
	{
		FString StartEndValue = WeaponAnimationUpdateData.bStart?"Start":"End";
		FString EventTypeName = USf_FunctionLibrary::GetEnumAsString<EWeaponAnimationEventType>(EventType);
		UE_LOG(
			Sf_WeaponAnimMontageController_FP,
			Warning,
			TEXT("No montage type found for Weapon Event Type %s %s"),
			*StartEndValue,
			*EventTypeName)
		return 0;
	}

	if (!WeaponAnimationUpdateData.bStart)
	{
		StopFPMontage(WeaponAnimationUpdateData.AnimData_TP,*MontageType);
		return 0;
	}

	return PlayFPMontage(WeaponAnimationUpdateData.AnimData_TP,*MontageType);
}

float USf_WeaponAnimMontageController_TP::PlayFPMontage(const FWeaponAnimData_TP& AnimData_TP, const FGameplayTag MontageType)
{
	bool bFoundMontage;
	UAnimMontage* AnimMontage =  UAnimDataHelper::GetAnimationMontage_TP(AnimData_TP,MontageType, bFoundMontage);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to play for %s"),*MontageType.GetTagName().ToString())
		return 0;
	}
	
	return PlayMontage(AnimMontage);
}

void USf_WeaponAnimMontageController_TP::StopFPMontage(const FWeaponAnimData_TP& AnimData_FP, const FGameplayTag MontageType)
{
	bool bFoundMontage;
	UAnimMontage* AnimMontage =  UAnimDataHelper::GetAnimationMontage_TP(AnimData_FP,MontageType, bFoundMontage);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to stop for %s"),*MontageType.GetTagName().ToString())
		return;
	}

	StopMontage(AnimMontage);
}
