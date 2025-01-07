#pragma once
#include "WeaponFeatureConfig.generated.h"

class UWeaponFeature;
class AWeaponBase;

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew,Abstract)
class UWeaponFeatureConfig: public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UWeaponFeature> GetFeatureType()  const;

	UFUNCTION()
	UWeaponFeature* MakeWeaponComponent(AWeaponBase* WeaponBase);
};

