#pragma once
#include "WF_CameraShake_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/FireBlocks.h"
#include "WF_CameraShake.generated.h"

UCLASS()
class UWF_CameraShake : public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
protected:
	//Overrides
	virtual void OnFire_Implementation() override;
protected:
	UPROPERTY()
	UWF_CameraShake_Config* Config;

	UPROPERTY()
	APlayerCameraManager* PlayerCameraManager;
};
