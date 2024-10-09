#include "WeaponAnimData_FP.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"

FWeaponAnimData_FP::FWeaponAnimData_FP()
{
	UpdateEntries();
}

void FWeaponAnimData_FP::UpdateEntries()
{
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationAssetType_FP,UAnimSequenceBase*>(AnimationAssets);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationMontageType_FP,UAnimMontage*>(AnimationMontages);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponBlendSpaceType_FP,UBlendSpace*>(Blendspaces);
}

UWeaponAnimationAsset_FP::UWeaponAnimationAsset_FP()
{
	WeaponAnimData.UpdateEntries();
}

void UWeaponAnimationAsset_FP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}

