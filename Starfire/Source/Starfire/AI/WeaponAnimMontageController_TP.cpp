#include "WeaponAnimMontageController_TP.h"

#include "Starfire/Animation/AnimationData/WeaponAnimDataHelper.h"
#include "Starfire/Animation/AnimationData/FP/WeaponAnimationMontageType_FP.h"
#include "Starfire/Animation/AnimationData/TP/WeaponAnimationMontageType_TP.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"


void USf_WeaponAnimMontageController_TP::InitializeComponent()
{
	Super::InitializeComponent();
}

void USf_WeaponAnimMontageController_TP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationEventType,FGameplayTag>(AnimationStartMappings);
}

float USf_WeaponAnimMontageController_TP::RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const
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

float USf_WeaponAnimMontageController_TP::PlayFPMontage(FWeaponAnimData_TP AnimData_TP,FGameplayTag MontageType) const
{
	UAnimMontage* AnimMontage =  UWeaponAnimDataHelper::GetAnimationMontage_TP(AnimData_TP,MontageType);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to play for %s"),*MontageType.GetTagName().ToString())
		return 0;
	}
	
	return PlayMontage(AnimMontage);
}

void USf_WeaponAnimMontageController_TP::StopFPMontage(FWeaponAnimData_TP AnimData_FP, FGameplayTag MontageType) const
{
	UAnimMontage* AnimMontage =  UWeaponAnimDataHelper::GetAnimationMontage_TP(AnimData_FP,MontageType);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to stop for %s"),*MontageType.GetTagName().ToString())
		return;
	}

	StopMontage(AnimMontage);
}
