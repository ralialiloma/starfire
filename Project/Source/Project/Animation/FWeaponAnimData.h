#pragma once
#include "WeaponAnimationAssetType.h"
#include "WeaponAnimationMontageType.h"
#include "FWeaponAnimData.generated.h"

USTRUCT(Blueprintable)
struct PROJECT_API FWeaponAnimData:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData();

	void UpdateEntries();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys))
	TMap<TEnumAsByte<EWeaponAnimationAssetType> ,UAnimationAsset*> AnimationAssets;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys))
	TMap<TEnumAsByte<EWeaponAnimationMontageType>,UAnimationAsset*> AnimationMontages;
};
