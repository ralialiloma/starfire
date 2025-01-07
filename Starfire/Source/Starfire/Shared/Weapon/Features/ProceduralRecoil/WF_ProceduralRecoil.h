#pragma once
#include "WF_ProceduralRecoil_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "WF_ProceduralRecoil.generated.h"

UCLASS()
class UWF_ProceduralRecoil : public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
	virtual void OnFire_Implementation() override;
	virtual void OnTick_Implementation(float OnTick) override;
	
public:
	UFUNCTION(BlueprintCallable)
	FVector GetActiveRecoilLocation() const;

	UFUNCTION(BlueprintCallable)
	FRotator GetActiveRecoilRotation() const;

	UFUNCTION(BlueprintCallable)
	FTransform GetCurrentRecoil() const;

protected:
	void ActivateRecoil();
	
protected:
	UPROPERTY()
	UWF_ProceduralRecoil_Config* Config;

	UPROPERTY(Transient)
	FVector ActiveRecoilLocation;

	UPROPERTY(Transient)
	FRotator ActiveRecoilRotation;

	
	UPROPERTY(Transient)
	FTransform CurrentRecoil;

		
	UPROPERTY(Transient)
	FTransform RecoilTarget;
};
