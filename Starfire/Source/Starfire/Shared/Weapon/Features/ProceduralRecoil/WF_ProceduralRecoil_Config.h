#pragma once
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "WF_ProceduralRecoil_Config.generated.h"

UCLASS()
class UWF_ProceduralRecoil_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const;
	
public:
	UPROPERTY(EditAnywhere)
	FVector RecoilLocationRangeMin = FVector(-0.16,-1.1f,0);

	UPROPERTY(EditAnywhere)
	FVector RecoilLocationRangeMax = FVector(0.16,-2.1f,0);

	UPROPERTY(EditAnywhere)
	FRotator RecoilRotationStrengthMin = FRotator(-0.8,-1.6f,-2.5);

	UPROPERTY(EditAnywhere)
	FRotator RecoilRotationStrengthMax  = FRotator(0.8f,1.6,-5);;

	UPROPERTY(EditAnywhere)
	float Multiplier = 1.0f;
};
