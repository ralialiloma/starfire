#include "FWeaponAnimData.h"

#include "Project/Utility/FunctionLibrary.h"

FWeaponAnimData::FWeaponAnimData()
{
	UpdateEntries();
}

void FWeaponAnimData::UpdateEntries()
{
	
	//Set up Weapon Animation Types
	TArray<EWeaponAnimationAssetType> WeaponAnimationAssetTypes =
		UFunctionLibrary::GetAllEnumValues<EWeaponAnimationAssetType>(true);
	for (EWeaponAnimationAssetType Type: WeaponAnimationAssetTypes)
	{
		if (!AnimationAssets.Contains(Type))
			AnimationAssets.Add(Type,nullptr);
	}
	
	//Set up Weapon Animation Types
	TArray<EWeaponAnimationMontageType> WeaponAnimationMontageTypes =
		UFunctionLibrary::GetAllEnumValues<EWeaponAnimationMontageType>(true);
	for (EWeaponAnimationMontageType Type: WeaponAnimationMontageTypes)
	{
		if (!AnimationMontages.Contains(Type))
			AnimationMontages.Add(Type,nullptr);
	}
		
}

UAnimSequenceBase* UWeaponAnimDataFunctions::GetAnimationAsset(FWeaponAnimData AnimData,
	EWeaponAnimationAssetType AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UWeaponAnimDataFunctions::GetAnimationMontage(FWeaponAnimData AnimData,
	EWeaponAnimationMontageType AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

