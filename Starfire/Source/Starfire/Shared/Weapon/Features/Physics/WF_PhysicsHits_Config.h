#pragma once
#include "GameplayTagContainer.h"
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WF_PhysicsHits_Config.generated.h"

UCLASS()
class UWF_PhysicsHits_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForceModifier = 1;
};
