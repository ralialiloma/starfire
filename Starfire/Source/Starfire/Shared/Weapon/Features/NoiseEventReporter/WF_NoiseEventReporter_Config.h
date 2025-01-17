#pragma once
#include "GameplayTagContainer.h"
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "WF_NoiseEventReporter_Config.generated.h"

UCLASS()
class UWF_NoiseEventReporter_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const override;
	
public:

	//Fire
	UPROPERTY(EditAnywhere)
	float FireVolume = 3.0f;
};
