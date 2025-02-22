// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_DamageController.h"

USf_DamageController::USf_DamageController(): bShouldPassiveHeal(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	MaxArmor = 1;
}


void USf_DamageController::BeginPlay()
{
	Super::BeginPlay();
	if (bStartWithMaxHealth)
		SetHealth(MaxHealth);
	CurrentArmor = MaxArmor;
	bShouldPassiveHeal = bEnablePassiveHealing;
}

void USf_DamageController::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentArmor = MaxArmor;
}

void USf_DamageController::TickComponent(const float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PassiveHeal(DeltaTime);
}



float USf_DamageController::ApplyDamage(
	const float Damage,
	const FVector HitLocation,
	const FVector HitNormal,
	UPrimitiveComponent* HitComponent,
	const FGameplayTag DamageType)
{
	//Get Hitbox
	const USf_Hitbox* Hitbox =  Cast<USf_Hitbox>(HitComponent);
	if (!IsValid(Hitbox) && IsValid(HitComponent))
		return 0;

	//Total Damage
	const float SafeCurrentArmor = FMath::Max(CurrentArmor,1);
	const float TotalDamage = (Damage/SafeCurrentArmor)*Hitbox->DamageMultiplier;
	
	return ApplyDamage(TotalDamage,HitLocation,HitNormal,DamageType);
}

void USf_DamageController::Reset()
{
	GetWorld()->GetTimerManager().PauseTimer(PassiveHealCooldown);
	GetWorld()->GetTimerManager().ClearTimer(PassiveHealCooldown);
	bShouldPassiveHeal = bEnablePassiveHealing;
	CurrentArmor = MaxArmor;
	//CurrentHealth = bStartWithMaxHealth ? MaxHealth : 0;
	SetHealth(bStartWithMaxHealth ? MaxHealth : 0);
}

void USf_DamageController::Heal(const float AmountOfHeal, const bool bInternal)
{
	if (CurrentHealth<=0 && !bInternal)
		return;
	
	const float AbsoluteAmountOfHeal = FMath::Abs(AmountOfHeal);
	const bool bRequiredHealing = CurrentHealth<MaxHealth;
	SetHealth(FMath::Min(CurrentHealth+AbsoluteAmountOfHeal,MaxHealth));
	if (bRequiredHealing && AbsoluteAmountOfHeal>0 && !bInternal)
	{
		OnHeal_CPP.Broadcast();
		OnHeal_BP.Broadcast();
	}
	if (CurrentHealth>=MaxHealth && !bInternal && bRequiredHealing)
	{
		OnFullHealth_CPP.Broadcast();
		OnFullHealth_BP.Broadcast();
	}
}

float USf_DamageController::IsMaxHealth() const
{
	return MaxHealth <= CurrentHealth;
}

float USf_DamageController::IsZeroHealth() const
{
	return CurrentHealth <= 0;
}


float USf_DamageController::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USf_DamageController::GetCurrentHealthInPercent() const
{
	if (MaxHealth > 0)
	{
		return CurrentHealth / MaxHealth;
	}

	return 0;
}

float USf_DamageController::GetCurrentArmor() const
{
	return CurrentArmor;
}

void USf_DamageController::SetHealth(const float NewHealth)
{
	const float OldHealth = CurrentHealth;
	LastHealth = OldHealth;
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	//Broadcast Health Change
	if (!FMath::IsNearlyEqual(NewHealth, OldHealth, 0.001f))
	{
		OnHealthChanged_CPP.Broadcast();
		OnHealthChanged_BP.Broadcast();
	}

	//Broadcast Max Health
	if (CurrentHealth>=MaxHealth && OldHealth<=MaxHealth)
	{
		OnFullHealth_CPP.Broadcast();
		OnFullHealth_BP.Broadcast();
	}

	//Broadcast Death
	if (CurrentHealth<=0 && OldHealth>0)
	{
		OnZeroHealth_CPP.Broadcast();
		OnZeroHealth_BP.Broadcast();
	}
}

float USf_DamageController::ApplyDamage(const float TotalDamage, const FVector& HitLocation, const FVector& HitNormal, const FGameplayTag DamageType)
{
	if (!SupportedDamageTypes.HasTag(DamageType))
		return 0;
	
	if (CurrentHealth<=0)
	{
		OnDeathDamage_BP.Broadcast();
		OnDeathDamage_CPP.Broadcast();
		return 0;
	}
	
	//Update CurrentHealth
	SetHealth(CurrentHealth-TotalDamage);

	//Stop Passive Heal
	bShouldPassiveHeal = false;
	GetWorld()->GetTimerManager().PauseTimer(PassiveHealCooldown);
	GetWorld()->GetTimerManager().ClearTimer(PassiveHealCooldown);
	GetWorld()->GetTimerManager().SetTimer(
		PassiveHealCooldown,
		[this]()->void{bShouldPassiveHeal = bEnablePassiveHealing && CurrentHealth>0;},
		PassiveHealingStartAfterDamageInSeconds,
		false);


	//Broadcast Damage Received
	OnDamageReceived_CPP.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal,DamageType);
	OnDamageReceived_BP.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal,DamageType);
	
	return TotalDamage;
}

float USf_DamageController::GetLastHealth() const
{
	return LastHealth;
}

float USf_DamageController::Kill(
	const FVector& HitLocation,
	const FVector& HitNormal,
	const FGameplayTag DamageType)
{
	const float CachedCurrentHealth = GetCurrentHealth();
	return ApplyDamage(CachedCurrentHealth,HitLocation,HitNormal,DamageType);;
}


void USf_DamageController::PassiveHeal(const float DeltaSeconds)
{
	if (bShouldPassiveHeal && PassiveHealingRatePerSecond > 0.f)
	{
		Heal(PassiveHealingRatePerSecond*DeltaSeconds);
	}
}



