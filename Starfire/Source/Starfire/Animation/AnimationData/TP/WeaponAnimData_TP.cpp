#include "WeaponAnimData_TP.h"
#include "Starfire/StarFireGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"

FWeaponAnimData_TP::FWeaponAnimData_TP()
{
	UpdateEntries();
}

void FWeaponAnimData_TP::UpdateEntries()
{
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimSequence*>(
		AnimationSequences_UB,
		Sf_GameplayTags::Animation::Character::TP::AnimSequence::UpperBody::Name);

	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimSequence*>(
	AnimationSequences_LB,
	Sf_GameplayTags::Animation::Character::TP::AnimSequence::LowerBody::Name);

	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimSequence*>(
	AnimationSequences_FB,
	Sf_GameplayTags::Animation::Character::TP::AnimSequence::FullBody::Name);
	
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimMontage*>(
		AnimationMontages,
		Sf_GameplayTags::Animation::Character::TP::Montage::Name);
	
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UBlendSpace*>(
		Blendspaces,
		Sf_GameplayTags::Animation::Character::TP::Blendspace::Name);
}

TMap<FGameplayTag, UAnimSequence*> FWeaponAnimData_TP::GetAllSequences() const
{
	TMap<FGameplayTag, UAnimSequence*>  AllSequences{};
	AllSequences.Append(AnimationSequences_UB);
	AllSequences.Append(AnimationSequences_LB);
	AllSequences.Append(AnimationSequences_FB);
	return AllSequences;
}


UWeaponAnimationAsset_TP::UWeaponAnimationAsset_TP()
{
}

#if WITH_EDITOR
void UWeaponAnimationAsset_TP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}
#endif





