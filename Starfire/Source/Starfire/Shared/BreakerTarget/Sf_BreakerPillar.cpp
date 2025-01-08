#include "Sf_BreakerPillar.h"
#include "Templates/Function.h"

ASf_BreakerPillar::ASf_BreakerPillar()
{
	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));
	DamageController->bEnablePassiveHealing = false;
	DamageController->SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::Explosion);
	DamageController->bStartWithMaxHealth = false;
	DamageController->MaxHealth = 1;

	//Hitbox
	PrimaryHitbox = CreateDefaultSubobject<USf_Hitbox>(TEXT("Hitbox"));
	PrimaryHitbox->SetUsingAbsoluteLocation(false);
	PrimaryHitbox->SetUsingAbsoluteRotation(false);
	PrimaryHitbox->SetUsingAbsoluteScale(false);

	RootComponent = PrimaryHitbox;
}

void ASf_BreakerPillar::BeginPlay()
{
	Super::BeginPlay();

	DamageController->OnZeroHealth_CPP.AddLambda([this]()
	{
		BreakPillar();
	});
}

USf_DamageController* ASf_BreakerPillar::GetDamageController() const
{
	return DamageController;
}

bool ASf_BreakerPillar::IsActive() const
{
	if (IsValid(DamageController))
		return !DamageController->IsZeroHealth();
	return false;
}

bool ASf_BreakerPillar::SetRestore(float RestoreValue, float& OutRemainingValue)
{
	if (!IsValid(this))
		return false;
	
	OutRemainingValue = RestoreValue;
	
	if (IsActive())
		return false;

	CurrentRestoreValue = FMath::Min(RestoreValue, MaxRestoreValue);
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
	DamageController->SetHealth(1);
	OnRestore();
}

void ASf_BreakerPillar::BreakPillar()
{
	OnBreak();
	OnBreak_BP.Broadcast();
	OnBreak_CPP.Broadcast();
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