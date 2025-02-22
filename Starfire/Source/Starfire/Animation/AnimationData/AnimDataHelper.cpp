﻿#include "AnimDataHelper.h"

#include "Weapon/WeaponAnimData_Weapon.h"


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

UAnimMontage* UAnimDataHelper::GetAnimationMontage_Weapon(FWeaponAnimData_Weapon AnimData,
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


UAnimSequence* UAnimDataHelper::GetAnimationSequence_TP(const FWeaponAnimData_TP AnimData, const FGameplayTag Tag, bool &bIsValid)
{
	UAnimSequence* FoundSequence = nullptr;
	UAnimSequence** FoundSequencePtr = AnimData.GetAllSequences().Find(Tag);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	bIsValid  = FoundSequence != nullptr;
	return FoundSequence;
}

UAnimSequence* UAnimDataHelper::GetSafeAnimationSequence_TP(const FWeaponAnimData_TP AnimData, const FGameplayTag Tag,  UAnimSequence* BackupSequence)
{
	bool bIsValid = false;
	UAnimSequence* FoundSequence =  GetAnimationSequence_TP(AnimData, Tag,  bIsValid);
	if (bIsValid)
	{
		return FoundSequence;
	}

	return BackupSequence;
}

void UAnimDataHelper::ReplaceEmptyWithDefaultsTP(FWeaponAnimData_TP AnimData, UAnimSequence* DefautAnimSequene, UBlendSpace* DefaultBlendSpace)
{
	
}

void UAnimDataHelper::ReplaceEmptyWithDefaults(FWeaponAnimData_TP& AnimData, UAnimSequence* DefaultAnimSequence, UBlendSpace* DefaultBlendSpace)
{
	for (TTuple<FGameplayTag, UAnimSequence*>& Entry : AnimData.AnimationSequences_FB)
	{
		if (!Entry.Value)
		{
			Entry.Value = DefaultAnimSequence;
		}
	}
	for (TTuple<FGameplayTag, UAnimSequence*>& Entry : AnimData.AnimationSequences_LB)
	{
		if (!Entry.Value)
		{
			Entry.Value = DefaultAnimSequence;
		}
	}

	for (TTuple<FGameplayTag, UAnimSequence*>& Entry : AnimData.AnimationSequences_UB)
	{
		if (!Entry.Value)
		{
			Entry.Value = DefaultAnimSequence;
		}
	}
	
	for (TTuple<FGameplayTag, UBlendSpace*>& Entry : AnimData.Blendspaces)
	{
		if (!Entry.Value)
		{
			Entry.Value = DefaultBlendSpace;
		}
	}
}

UAnimMontage* UAnimDataHelper::GetAnimationMontage_TP(FWeaponAnimData_TP AnimData, const FGameplayTag AssetType, bool &bIsValid)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	bIsValid  = FoundMontage != nullptr;
	return FoundMontage;
}

UBlendSpace* UAnimDataHelper::GetSafeBlendspace_TP(const FWeaponAnimData_TP& AnimData, const FGameplayTag AssetType,UBlendSpace* BackupBlendSpace)
{
	bool bIsValid = false;
	UBlendSpace* FoundBlendspace =  GetBlendspace_TP(AnimData, AssetType,  bIsValid);
	if (bIsValid)
		return FoundBlendspace;
	return BackupBlendSpace;
}

UBlendSpace* UAnimDataHelper::GetBlendspace_TP(FWeaponAnimData_TP AnimData, const FGameplayTag AssetType,bool &bIsValid)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	bIsValid  = FoundBlendspace != nullptr;
	return FoundBlendspace;
}