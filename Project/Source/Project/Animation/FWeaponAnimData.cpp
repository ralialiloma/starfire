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
		UFunctionLibrary::GetAllEnumValues<EWeaponAnimationAssetType>();
	for (EWeaponAnimationAssetType Type: WeaponAnimationAssetTypes)
	{
		if (!AnimationAssets.Contains(Type))
			AnimationAssets.Add(Type,nullptr);
	}
	
	//Set up Weapon Animation Types
	TArray<EWeaponAnimationMontageType> WeaponAnimationMontageTypes =
		UFunctionLibrary::GetAllEnumValues<EWeaponAnimationMontageType>();
	for (EWeaponAnimationMontageType Type: WeaponAnimationMontageTypes)
	{
		if (!AnimationMontages.Contains(Type))
			AnimationMontages.Add(Type,nullptr);
	}
		
}
