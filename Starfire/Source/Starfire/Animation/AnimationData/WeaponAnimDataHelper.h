#pragma once
#include "FP/WeaponAnimData_FP.h"
#include "TP/WeaponAnimData_TP.h"
#include "WeaponAnimDataHelper.generated.h"


UCLASS()
class STARFIRE_API UWeaponAnimDataHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequenceBase* GetAnimationAsset_TP(FWeaponAnimData_TP AnimData,EWeaponAnimationAssetType_TP AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_TP(FWeaponAnimData_TP AnimData, EWeaponAnimationMontageType_TP AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation", meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_TP(FWeaponAnimData_TP AnimData, EWeaponBlendSpaceType_TP AssetType);

	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequenceBase* GetAnimationAsset_FP(FWeaponAnimData_FP AnimData,EWeaponAnimationAssetType_FP AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_FP(FWeaponAnimData_FP AnimData, EWeaponAnimationMontageType_FP AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation", meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_FP(FWeaponAnimData_FP AnimData, EWeaponBlendSpaceType_FP AssetType);

	
};
