#include "Sf_BreakerPillar.h"
#include "Templates/Function.h"

ASf_BreakerPillar::ASf_BreakerPillar()
{
	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));
	DamageController->bEnablePassiveHealing = true;
	DamageController->SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::Explosion);
	DamageController->bStartWithMaxHealth = true;
	DamageController->MaxHealth = 1;

	//Hitbox
	PrimaryHitbox = CreateDefaultSubobject<USf_Hitbox>(TEXT("Hitbox"));
	PrimaryHitbox->SetUsingAbsoluteLocation(false);
	PrimaryHitbox->SetUsingAbsoluteRotation(false);
	PrimaryHitbox->SetUsingAbsoluteScale(false);

	RootComponent = PrimaryHitbox;
}

USf_DamageController* ASf_BreakerPillar::GetDamageController() const
{
	return DamageController;
}

bool ASf_BreakerPillar::IsActive() const
{
	return !DamageController->IsZeroHealth();
}

bool ASf_BreakerPillar::SetRestore(float RestoreValue, float& OutRemainingValue)
{
	OutRemainingValue = RestoreValue;
	
	if (IsActive())
		return false;

	CurrentRestoreValue = FMath::Min(CurrentRestoreValue, MaxRestoreValue);
	OutRemainingValue = FMath::Max(RestoreValue - MaxRestoreValue, 0.0f);

	OnRestoreProgress(FMath::Clamp(CurrentRestoreValue / MaxRestoreValue, 0.f, 1.f));

	if (OutRemainingValue > 0.0f)
		FullRestore();
	
	return OutRemainingValue > 0.0f;
}

void ASf_BreakerPillar::FullRestore()
{
	if (IsActive())
		return;

	CurrentRestoreValue = 0;
	DamageController->Reset();
	OnRestore();
}

void ASf_BreakerPillar::OnBreak_Implementation()
{
}

void ASf_BreakerPillar::OnRestore_Implementation()
{
}

void ASf_BreakerPillar::OnRestoreProgress_Implementation(float Alpha)
{
}