#pragma once
#include "GameplayTagContainer.h"
#include "Starfire/Shared/Weapon/WeaponFeatureConfig.h"
#include "WF_ActionLogger_Config.generated.h"

UCLASS()
class UWF_ActionLogger_Config: public UWeaponFeatureConfig
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UWeaponFeature> GetFeatureType_Implementation() const override;
	
public:

	//Fire
	UPROPERTY(EditAnywhere, Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag StartFire;

	/*UPROPERTY(EditAnywhere,Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag TickFire;*/
	
	/*UPROPERTY(EditAnywhere,Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag StopFire;*/

	UPROPERTY(EditAnywhere,Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag FireWithoutAmmo;

	UPROPERTY(EditAnywhere,Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag FireAndKill;

	UPROPERTY(EditAnywhere,Category = "Fire",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag FireAndApplyDamage;

	//Reload
	UPROPERTY(EditAnywhere,Category = "Reload",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag StartReload;

	UPROPERTY(EditAnywhere,Category = "Reload",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag StopReload;

	UPROPERTY(EditAnywhere,Category = "Reload",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag FinishReload;
	
	//Reload
	/*UPROPERTY(EditAnywhere,Category = "Melee",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag StartMelee;

	UPROPERTY(EditAnywhere,Category = "Melee",meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag EndMelee;*/
};
