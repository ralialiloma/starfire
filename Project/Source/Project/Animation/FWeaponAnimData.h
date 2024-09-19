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
	TMap<TEnumAsByte<EWeaponAnimationAssetType> ,UAnimSequenceBase*> AnimationAssets;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys))
	TMap<TEnumAsByte<EWeaponAnimationMontageType>,UAnimMontage*> AnimationMontages;
};

UCLASS()
class PROJECT_API UWeaponAnimDataFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation")
	static UAnimSequenceBase* GetAnimationAsset(FWeaponAnimData AnimData,EWeaponAnimationAssetType AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation")
	static UAnimMontage* GetAnimationMontage(FWeaponAnimData AnimData, EWeaponAnimationMontageType AssetType);
};

