#pragma once
#include "WF_PhysicsHits_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "WF_PhysicsHits.generated.h"

UCLASS()
class UWF_PhysicsHits: public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
	virtual void OnHitObject_Implementation(FHitResult HitResult) override;

protected:
	UPROPERTY()
	UWF_PhysicsHits_Config* Config;
};
