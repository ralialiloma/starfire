#include "Sf_BreakerShard.h"

ASf_BreakerShard::ASf_BreakerShard()
{
	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));
	DamageController->bEnablePassiveHealing = false;
	DamageController->SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::Explosion);
	DamageController->bStartWithMaxHealth = false;

	//Hitbox
	PrimaryHitbox = CreateDefaultSubobject<USf_Hitbox>(TEXT("Hitbox"));
	PrimaryHitbox->SetUsingAbsoluteLocation(false);
	PrimaryHitbox->SetUsingAbsoluteRotation(false);
	PrimaryHitbox->SetUsingAbsoluteScale(false);
}

void ASf_BreakerShard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

USf_DamageController* ASf_BreakerShard::GetDamageController() const
{
	return DamageController;
}


