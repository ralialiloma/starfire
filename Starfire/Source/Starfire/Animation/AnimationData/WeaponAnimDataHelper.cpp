#include "WeaponAnimDataHelper.h"


UAnimSequenceBase* UWeaponAnimDataHelper::GetAnimationAsset_FP(FWeaponAnimData_FP AnimData,
															   EWeaponAnimationAssetType_FP AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UWeaponAnimDataHelper::GetAnimationMontage_FP(FWeaponAnimData_FP AnimData,
	EWeaponAnimationMontageType_FP AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UWeaponAnimDataHelper::GetBlendspace_FP(FWeaponAnimData_FP AnimData, EWeaponBlendSpaceType_FP AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}


UAnimSequence* UWeaponAnimDataHelper::GetAnimationSequence_TP(FWeaponAnimData_TP AnimData, FGameplayTag Tag)
{
	UAnimSequence* FoundSequence = nullptr;
	UAnimSequence** FoundSequencePtr = AnimData.GetAllSequences().Find(Tag);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UWeaponAnimDataHelper::GetAnimationMontage_TP(FWeaponAnimData_TP AnimData, FGameplayTag AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UWeaponAnimDataHelper::GetBlendspace_TP(FWeaponAnimData_TP AnimData, FGameplayTag AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}