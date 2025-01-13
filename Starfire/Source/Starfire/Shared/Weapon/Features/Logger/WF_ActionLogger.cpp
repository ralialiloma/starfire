#include "WF_ActionLogger.h"

#include "Starfire/Shared/ActionLogger/ActionLogger.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_ActionLogger::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_ActionLogger_Config,Config);

	//Reload
	GetOwningWeapon()->OnWeaponApplyDamage.AddDynamic(this,&UWF_ActionLogger::OnWeaponApplyDamage);
	GetOwningWeapon()->OnReloadStopped_BP.AddDynamic(this,&UWF_ActionLogger::OnReloadStopped);
	GetOwningWeapon()->OnReloadFinish_BP.AddDynamic(this,&UWF_ActionLogger::OnReloadFinished);

	GetOwningWeapon()->OnFailFire.AddDynamic(this, &UWF_ActionLogger::OnFailFire);
}

void UWF_ActionLogger::OnWeaponApplyDamage(const float Damage, FGameplayTag DamageType, FHitResult HitResult, USf_DamageController* DamageController)
{
	ReportAction(Config->FireAndApplyDamage);
	if (DamageController->GetCurrentHealth()<0)
	{
		ReportAction(Config->FireAndKill);
	}
}


void UWF_ActionLogger::ReportAction(const FGameplayTag ActionTag)
{
	const FActionLog ActionLog =  FActionLog(ActionTag, GetOwningWeapon()->GetRootComponent()->GetComponentLocation());
	UActionLoggerSubSystem::Get(GetWorld())->ReportAction(ActionLog);
}

void UWF_ActionLogger::OnReloadStart()
{
	ReportAction(Config->StartReload);
}

void UWF_ActionLogger::OnReloadStopped()
{
	ReportAction(Config->StopReload);
}

void UWF_ActionLogger::OnReloadFinished()
{
	ReportAction(Config->FinishReload);
}

void UWF_ActionLogger::OnFailFire(const EFireBlock FireBlock)
{
	if (FireBlock == EFireBlock::EmptyClip || FireBlock == EFireBlock::NotEnoughAmmo)
		ReportAction(Config->FireWithoutAmmo);
}


