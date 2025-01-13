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
	UFUNCTION()
	void OnWeaponApplyDamage (const float Damage,FGameplayTag DamageType,FHitResult HitResult,USf_DamageController* DamageController);

	void ReportAction(FGameplayTag ActionTag);

	UFUNCTION()
	void OnReloadStart();
	UFUNCTION()
	void OnReloadStopped();
	UFUNCTION()
	void OnReloadFinished();

	UFUNCTION()
	void OnFailFire(EFireBlock FireBlock);
protected:
	UPROPERTY()
	UWF_ActionLogger_Config* Config;
};
