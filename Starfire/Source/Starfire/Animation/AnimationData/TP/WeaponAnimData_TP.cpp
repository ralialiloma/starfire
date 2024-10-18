#include "WeaponAnimData_TP.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"

FWeaponAnimData_TP::FWeaponAnimData_TP()
{
	UpdateEntries();
}

void FWeaponAnimData_TP::UpdateEntries()
{
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationAssetType_UB_TP,UAnimSequenceBase*>(AnimationAssets_UB);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationAssetType_LB_TP,UAnimSequenceBase*>(AnimationAssets_LB);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationAssetType_FB_TP,UAnimSequenceBase*>(AnimationAssets_FB);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationMontageType_TP,UAnimMontage*>(AnimationMontages);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponBlendSpaceType_TP,UBlendSpace*>(Blendspaces);
}

UWeaponAnimationAsset_TP::UWeaponAnimationAsset_TP()
{
}

void UWeaponAnimationAsset_TP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}





