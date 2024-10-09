#include "WeaponAnimMontageController_FP.h"

#include "Starfire/Animation/AnimationData/WeaponAnimDataHelper.h"
#include "Starfire/Animation/AnimationData/FP/WeaponAnimationMontageType_FP.h"
#include "Starfire/Animation/AnimationData/TP/WeaponAnimationMontageType_TP.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


void USf_WeaponAnimMontageController_FP::InitializeComponent()
{
	Super::InitializeComponent();
}

void USf_WeaponAnimMontageController_FP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationEventType,EWeaponAnimationMontageType_FP>(AnimationStartMappings);
}

float USf_WeaponAnimMontageController_FP::RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const
{
	const EWeaponAnimationEventType EventType = WeaponAnimationUpdateData.AnimationEventType;
	const EWeaponAnimationMontageType_FP* MontageType  =  AnimationStartMappings.Find(EventType);
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
		StopFPMontage(WeaponAnimationUpdateData.AnimData_FP,*MontageType);
		return 0;
	}

	return PlayFPMontage(WeaponAnimationUpdateData.AnimData_FP,*MontageType);
}

float USf_WeaponAnimMontageController_FP::PlayFPMontage(FWeaponAnimData_FP AnimData_FP,EWeaponAnimationMontageType_FP MontageType) const
{
	UAnimMontage* AnimMontage =  UWeaponAnimDataHelper::GetAnimationMontage_FP(AnimData_FP,MontageType);
	if (!IsValid(AnimMontage))
	{
		FString MontageTypeName = USf_FunctionLibrary::GetEnumAsString<EWeaponAnimationMontageType_FP>(MontageType);
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to play for %s"),*MontageTypeName)
		return 0;
	}
	
	return PlayMontage(AnimMontage);
}

void USf_WeaponAnimMontageController_FP::StopFPMontage(FWeaponAnimData_FP AnimData_FP, EWeaponAnimationMontageType_FP MontageType) const
{
	UAnimMontage* AnimMontage =  UWeaponAnimDataHelper::GetAnimationMontage_FP(AnimData_FP,MontageType);
	if (!IsValid(AnimMontage))
	{
		FString MontageTypeName = USf_FunctionLibrary::GetEnumAsString<EWeaponAnimationMontageType_FP>(MontageType);
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to stop for %s"),*MontageTypeName)
		return;
	}

	StopMontage(AnimMontage);
}
