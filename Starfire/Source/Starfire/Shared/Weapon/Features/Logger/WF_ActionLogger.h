#pragma once
#include "WF_ActionLogger_Config.h"
#include "Starfire/Shared/Weapon/WeaponFeature.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/FireBlocks.h"
#include "WF_ActionLogger.generated.h"

UCLASS()
class UWF_ActionLogger : public UWeaponFeature
{
	GENERATED_BODY()

public:
	virtual void OnInit_Implementation() override;
protected:

	//Subscriptions
	UFUNCTION()
	void OnWeaponApplyDamage(const float Damage,FGameplayTag DamageType,FHitResult HitResult,USf_DamageController* DamageController);
	UFUNCTION()
	void OnFailFire(EFireBlock FireBlock);
	UFUNCTION()
	void OnReloadStopped();
	UFUNCTION()
	void OnReloadFinished();
	
	//Overrides
	virtual void OnFire_Implementation() override;
	virtual void OnReload_Implementation() override;
	
	//Helper
	void ReportAction(FGameplayTag ActionTag);

protected:
	UPROPERTY()
	UWF_ActionLogger_Config* Config;
};
