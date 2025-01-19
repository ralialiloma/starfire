#include "WeaponAnimData_FP.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"

FWeaponAnimData_FP::FWeaponAnimData_FP()
{
	UpdateEntries();
}

void FWeaponAnimData_FP::UpdateEntries()
{
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimSequence*>(
		AnimationSequences,
		Sf_GameplayTags::Animation::Character::FP::AnimSequence::Name);
	
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimMontage*>(
		AnimationMontages,
		Sf_GameplayTags::Animation::Character::FP::Montage::Name);
	
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UBlendSpace*>(
		Blendspaces,
		Sf_GameplayTags::Animation::Character::FP::Blendspace::Name);
}

UWeaponAnimationAsset_FP::UWeaponAnimationAsset_FP()
{
	WeaponAnimData.UpdateEntries();
}

#if WITH_EDITOR
void UWeaponAnimationAsset_FP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}

void UWeaponAnimationAsset_FP::UpdateEntries()
{
	WeaponAnimData.UpdateEntries();
}
#endif

