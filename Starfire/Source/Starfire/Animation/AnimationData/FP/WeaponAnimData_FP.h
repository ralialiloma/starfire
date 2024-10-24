#pragma once
#include "GameplayTagContainer.h"
#include "WeaponAnimationAssetType_FP.h"
#include "WeaponAnimationMontageType_FP.h"
#include "WeaponBlendSpaceType_FP.h"
#include "WeaponAnimData_FP.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponAnimData_FP:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData_FP();

	void UpdateEntries();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag ,UAnimSequence*> AnimationSequences;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag, UAnimMontage*> AnimationMontages;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag, UBlendSpace*> Blendspaces;
};


UCLASS(Blueprintable)
class STARFIRE_API UWeaponAnimationAsset_FP:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset_FP();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData_FP WeaponAnimData;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};



