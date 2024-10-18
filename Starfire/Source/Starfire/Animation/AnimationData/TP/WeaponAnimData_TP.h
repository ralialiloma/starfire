#pragma once
#include "WeaponAnimationAssetType_FB_TP.h"
#include "WeaponAnimationAssetType_LB_TP.h"
#include "WeaponAnimationAssetType_UB_TP.h"
#include "WeaponAnimationMontageType_TP.h"
#include "WeaponBlendSpaceType_TP.h"
#include "WeaponAnimData_TP.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponAnimData_TP:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData_TP();

	void UpdateEntries();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<EWeaponAnimationAssetType_UB_TP ,UAnimSequenceBase*> AnimationAssets_UB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<EWeaponAnimationAssetType_LB_TP ,UAnimSequenceBase*> AnimationAssets_LB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<EWeaponAnimationAssetType_FB_TP ,UAnimSequenceBase*> AnimationAssets_FB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<EWeaponAnimationMontageType_TP, UAnimMontage*> AnimationMontages;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<EWeaponBlendSpaceType_TP, UBlendSpace*> Blendspaces;
};


UCLASS(Blueprintable)
class STARFIRE_API UWeaponAnimationAsset_TP:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset_TP();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData_TP WeaponAnimData;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};

