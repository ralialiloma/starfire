#pragma once
#include "WeaponAnimationAssetType.h"
#include "WeaponAnimationMontageType.h"
#include "WeaponBlendSpaceType.h"
#include "Project/Utility/FunctionLibrary.h"
#include "FWeaponAnimData.generated.h"

USTRUCT(Blueprintable)
struct PROJECT_API FWeaponAnimData:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData();

	void UpdateEntries();

	template<typename EnumType, typename AssetType>
	void UpdateEntry(TMap<TEnumAsByte<EnumType>, AssetType>& AssetMap);

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<TEnumAsByte<EWeaponAnimationAssetType> ,UAnimSequenceBase*> AnimationAssets;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<TEnumAsByte<EWeaponAnimationMontageType>,UAnimMontage*> AnimationMontages;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<TEnumAsByte<EWeaponBlendSpaceType>,UBlendSpace*> Blendspaces;
};

template <typename EnumType, typename AssetType>
void FWeaponAnimData::UpdateEntry(TMap<TEnumAsByte<EnumType>, AssetType>& AssetMap)
{
	static_assert(TIsEnum<EnumType>::Value, "UpdateEntry can only be used with enum types!");

	TArray<EnumType> AssetTypes = UFunctionLibrary::GetAllEnumValues<EnumType>(true);

	// Import New Enums
	for (EnumType Type : AssetTypes)
	{
		if (!AssetMap.Contains(Type))
			AssetMap.Add(Type, nullptr);
	}

	// Remove Empty Enum Entries (None)
	AssetMap.Remove(TEnumAsByte<EnumType>(0));

	// Collect Invalid Enum Keys for Removal
	const UEnum* EnumInfo = StaticEnum<EnumType>();
	TArray<TEnumAsByte<EnumType>> KeysToRemove;

	for (const auto& Kvp : AssetMap)
	{
		if (!EnumInfo->IsValidEnumValue(Kvp.Key))
		{
			KeysToRemove.Add(Kvp.Key);
		}
	}

	// Remove invalid keys outside of the iteration
	for (const auto& Key : KeysToRemove)
	{
		AssetMap.Remove(Key);
	}
}

UCLASS(Blueprintable)
class PROJECT_API UWeaponAnimationAsset:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData WeaponAnimData;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};

UCLASS()
class PROJECT_API UWeaponAnimDataFunctions : public UBlueprintFunctionLibrary
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

