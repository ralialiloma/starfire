// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_DamageController.h"

USf_DamageController::USf_DamageController()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	MaxArmor = 1;
}


void USf_DamageController::BeginPlay()
{
	Super::BeginPlay();
	if (bStartWithMaxHealth)
		SetHealth(MaxHealth);;
	CurrentArmor = MaxArmor;
	if (bPassiveHealing)
		GetWorld()->GetTimerManager().SetTimer(PassiveHealTimer,[this]()->void{Heal(PassiveHealingRate);},1.0f,true);
}

void USf_DamageController::InitializeComponent()
{
	Super::InitializeComponent();
	if (bStartWithMaxHealth)
		SetHealth(MaxHealth);;
	CurrentArmor = MaxArmor;
}

void USf_DamageController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


float USf_DamageController::ApplyDamage(
	const float Damage,
	const FVector HitLocation,
	const FVector HitNormal,
	UPrimitiveComponent* HitComponent,
	const FGameplayTag DamageType)
{
	if (!SupportedDamageTypes.HasTag(DamageType))
		return 0;
	
	if (CurrentHealth<=0)
		return 0;
	
	//Get Hitbox
	const USf_Hitbox* Hitbox =  Cast<USf_Hitbox>(HitComponent);
	if (!IsValid(Hitbox))
		return 0;
	
	//Total Damage
	float SafeCurrentArmor = FMath::Max(CurrentArmor,1);
	float TotalDamage = (Damage/SafeCurrentArmor)*Hitbox->DamageMultiplier;

	//Update CurrentHealth
	SetHealth(FMath::Max(CurrentHealth-TotalDamage,0));

	//Broadcast Damage Received
	OnDamageReceived_CPP.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal);
	OnDamageReceived_BP.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal);
	
	return TotalDamage;
}

void USf_DamageController::Heal(const float AmountOfHeal, const bool bInternal)
{
	const bool bRequiredHealing = CurrentHealth<MaxHealth;
	SetHealth(FMath::Min(CurrentHealth+AmountOfHeal,MaxHealth)) ;
	if (bRequiredHealing && AmountOfHeal>0 && !bInternal)
	{
		OnHeal_CPP.Broadcast();
		OnHeal_BP.Broadcast();
	}
	if (CurrentHealth>=MaxHealth && !bInternal && bRequiredHealing)
	{
		OnFullHealth_BP.Broadcast();
		OnFullHealth_CPP.Broadcast();
	}
}

void USf_DamageController::RestoreHealth()
{
	CurrentHealth = MaxHealth;
}


float USf_DamageController::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USf_DamageController::GetCurrentHealthInPercent() const
{
	return CurrentHealth/MaxHealth;
}

float USf_DamageController::GetCurrentArmor() const
{
	return CurrentArmor;
}

void USf_DamageController::SetHealth(const float NewHealth)
{
	const float OldHealth = CurrentHealth;
	CurrentHealth = NewHealth;

	//Broadcast Health Change
	if (!FMath::IsNearlyEqual(NewHealth, OldHealth, 0.01f))
	{
		OnHealthChanged_CPP.Broadcast();
		OnHealthChanged_BP.Broadcast();
	}

	//Broadcast Max Health
	if (CurrentHealth>=MaxHealth && OldHealth<=MaxHealth)
	{
		OnFullHealth_BP.Broadcast();
		OnFullHealth_CPP.Broadcast();
	}

	//Broadcast Death
	if (CurrentHealth<=0)
	{
		OnZeroHealth_BP.Broadcast();
		OnZeroHealth_CPP.Broadcast();
	}
}



