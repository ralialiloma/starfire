#pragma once
#include "GameplayTagContainer.h"
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "WF_CameraShake_Config.generated.h"

UCLASS()
class UWF_CameraShake_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const override;
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> ShakeClass; 

	UPROPERTY(EditAnywhere)
	float Scale = 1;

	UPROPERTY(EditAnywhere)
	ECameraShakePlaySpace PlaySpace = ECameraShakePlaySpace::CameraLocal;
};
