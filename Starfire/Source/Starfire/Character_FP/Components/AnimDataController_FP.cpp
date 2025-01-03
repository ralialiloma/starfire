#include "AnimDataController_FP.h"

#include "Starfire/Animation/AnimationData/AnimDataHelper.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


void USf_AnimDataController_FP::InitializeComponent()
{
	Super::InitializeComponent();
}

#if WITH_EDITOR
void USf_AnimDataController_FP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationEventType,FGameplayTag>(
		AnimationMontageMappings);
}
#endif

float USf_AnimDataController_FP::RunAnimation(const FWeaponMontageEventPackage WeaponAnimationUpdateData)
{
	const EWeaponAnimationEventType EventType = WeaponAnimationUpdateData.AnimationEventType;
	const FGameplayTag* MontageType  =  AnimationMontageMappings.Find(EventType);
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

float USf_AnimDataController_FP::PlayFPMontage(const FWeaponAnimData_FP& AnimData_FP, const FGameplayTag MontageType)
{
	UAnimMontage* AnimMontage =  UAnimDataHelper::GetAnimationMontage_FP(AnimData_FP,MontageType);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to play for %s"),*MontageType.GetTagName().ToString())
		return 0;
	}
	
	return PlayMontage(AnimMontage);
}

void USf_AnimDataController_FP::StopFPMontage(FWeaponAnimData_FP AnimData_FP, FGameplayTag MontageType)
{
	UAnimMontage* AnimMontage =  UAnimDataHelper::GetAnimationMontage_FP(AnimData_FP,MontageType);
	if (!IsValid(AnimMontage))
	{
		UE_LOG(Sf_WeaponAnimMontageController_FP, Warning, TEXT("Could not find montage to stop for %s"),*MontageType.GetTagName().ToString())
		return;
	}

	StopMontage(AnimMontage);
}
