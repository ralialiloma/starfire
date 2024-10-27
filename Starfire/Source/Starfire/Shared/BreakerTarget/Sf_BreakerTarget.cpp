#include "Sf_BreakerTarget.h"

#include "Starfire/Shared/Damage/Sf_DamageController.h"

ASf_BreakerTarget::ASf_BreakerTarget(const FObjectInitializer& ObjectInitializer): ProgressionRatePerSecond(0.05)
{
	//Damage Controller
	SfDamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("SfDamageController"));
	SfDamageController->bEnablePassiveHealing = false;
	SfDamageController->SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::Explosion);
	SfDamageController->bStartWithMaxHealth = false;

	//Damage Controller
	PrimaryHitbox = CreateDefaultSubobject<USf_Hitbox>(TEXT("HitBox"));
	PrimaryHitbox->SetUsingAbsoluteLocation(false);
	PrimaryHitbox->SetUsingAbsoluteRotation(false);
	PrimaryHitbox->SetUsingAbsoluteScale(false);
}

void ASf_BreakerTarget::BeginPlay()
{
	Super::BeginPlay();
	SfDamageController->OnHealthChanged_CPP.AddLambda([this]()->void
{
	OnProgressChanged_CPP.Broadcast();
	OnProgressChanged_BP.Broadcast();
});

	SfDamageController->OnFullHealth_CPP.AddLambda([this]()->void
	{
		OnFullProgress_CPP.Broadcast();
		OnFullProgress_BP.Broadcast();
	});
}

void ASf_BreakerTarget::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DoProgress(DeltaSeconds);
}

void ASf_BreakerTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

float ASf_BreakerTarget::GetProgress() const
{
	return SfDamageController->GetCurrentHealthInPercent();
}

USf_DamageController* ASf_BreakerTarget::GetDamageController() const
{
	return SfDamageController;
}

void ASf_BreakerTarget::DoProgress(const float DeltaSeconds)
{
	SfDamageController->Heal(ProgressionRatePerSecond*DeltaSeconds);
}

