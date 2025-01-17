#pragma once
#include "WF_NoiseEventReporter_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/FireBlocks.h"
#include "WF_NoiseEventReporter.generated.h"

UCLASS()
class UWF_NoiseEventReporter : public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
protected:
	//Overrides
	virtual void OnFire_Implementation() override;
protected:
	UPROPERTY()
	UWF_NoiseEventReporter_Config* Config;
};
