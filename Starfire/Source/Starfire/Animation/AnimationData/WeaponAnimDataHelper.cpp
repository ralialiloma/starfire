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


UAnimSequenceBase* UWeaponAnimDataHelper::GetAnimationAsset_UB_TP(FWeaponAnimData_TP AnimData, EWeaponAnimationAssetType_UB_TP AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets_UB.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimSequenceBase* UWeaponAnimDataHelper::GetAnimationAsset_LB_TP(FWeaponAnimData_TP AnimData, EWeaponAnimationAssetType_LB_TP AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets_LB.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}


UAnimSequenceBase* UWeaponAnimDataHelper::GetAnimationAsset_FB_TP(FWeaponAnimData_TP AnimData, EWeaponAnimationAssetType_FB_TP AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets_FB.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UWeaponAnimDataHelper::GetAnimationMontage_TP(FWeaponAnimData_TP AnimData, EWeaponAnimationMontageType_TP AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UWeaponAnimDataHelper::GetBlendspace_TP(FWeaponAnimData_TP AnimData, EWeaponBlendSpaceType_TP AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}