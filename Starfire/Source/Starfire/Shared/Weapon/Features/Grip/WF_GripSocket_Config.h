#pragma once
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WF_GripSocket_Config.generated.h"

UCLASS()
class UWF_GripSocket_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const override;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName SocketName;
};
