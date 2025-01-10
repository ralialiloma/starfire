#pragma once
#include "WF_HitVFX_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "WF_HitVFX.generated.h"

UCLASS()
class UWF_HitVFX: public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
	virtual void OnHitObject_Implementation(FHitResult HitResult) override;

protected:
	void PlayHitVFX(const UPhysicalMaterial* PhysicalMaterial, const FVector& HitLocation) const;

protected:
	UPROPERTY()
	UWF_HitVFX_Config* Config;
};
