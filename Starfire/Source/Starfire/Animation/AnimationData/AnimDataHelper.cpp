#include "AnimDataHelper.h"


UAnimSequenceBase* UAnimDataHelper::GetAnimationSequence_FP(FWeaponAnimData_FP AnimData,
															   FGameplayTag AssetType)
{
	UAnimSequence* FoundSequence = nullptr;
	UAnimSequence** FoundSequencePtr = AnimData.AnimationSequences.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UAnimDataHelper::GetAnimationMontage_FP(FWeaponAnimData_FP AnimData,
	FGameplayTag AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UAnimDataHelper::GetBlendspace_FP(FWeaponAnimData_FP AnimData, FGameplayTag AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}


UAnimSequence* UAnimDataHelper::GetAnimationSequence_TP(FWeaponAnimData_TP AnimData, FGameplayTag Tag)
{
	UAnimSequence* FoundSequence = nullptr;
	UAnimSequence** FoundSequencePtr = AnimData.GetAllSequences().Find(Tag);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UAnimDataHelper::GetAnimationMontage_TP(FWeaponAnimData_TP AnimData, FGameplayTag AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UAnimDataHelper::GetBlendspace_TP(FWeaponAnimData_TP AnimData, FGameplayTag AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}