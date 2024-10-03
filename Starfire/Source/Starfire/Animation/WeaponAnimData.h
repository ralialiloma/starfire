#pragma once
#include "WeaponAnimationAssetType.h"
#include "WeaponAnimationMontageType.h"
#include "WeaponBlendSpaceType.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "WeaponAnimData.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponAnimData:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData();

	void UpdateEntries();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<EWeaponAnimationAssetType ,UAnimSequenceBase*> AnimationAssets;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<EWeaponAnimationMontageType, UAnimMontage*> AnimationMontages;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<EWeaponBlendSpaceType, UBlendSpace*> Blendspaces;
};


UCLASS(Blueprintable)
class STARFIRE_API UWeaponAnimationAsset:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData WeaponAnimData;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};

UCLASS()
class STARFIRE_API UWeaponAnimDataFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequenceBase* GetAnimationAsset(FWeaponAnimData AnimData,EWeaponAnimationAssetType AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage(FWeaponAnimData AnimData, EWeaponAnimationMontageType AssetType);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation", meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace(FWeaponAnimData AnimData, EWeaponBlendSpaceType AssetType);
};

