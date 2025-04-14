#pragma once
#include "WF_GripSocket_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "WF_GripSocket.generated.h"

UCLASS()
class UWF_GripSocket: public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;

	UFUNCTION(BlueprintCallable)
	FTransform GetLeftHandGripTransform();

protected:
	UPROPERTY()
	UWF_GripSocket_Config* Config;
};
