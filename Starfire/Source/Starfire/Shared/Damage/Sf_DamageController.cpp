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
	CurrentHealth = MaxHealth;
	CurrentArmor = MaxArmor;
	GetWorld()->GetTimerManager().SetTimer(PassiveHealTimer,[this]()->void{Heal(HealRatePerSecond);},1.0f,true);
}

void USf_DamageController::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentHealth = MaxHealth;
	CurrentArmor = MaxArmor;
}

void USf_DamageController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


float USf_DamageController::ApplyDamage(const float Damage,FVector HitLocation, FVector HitNormal,UPrimitiveComponent* HitComponent)
{
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
	CurrentHealth = FMath::Max(CurrentHealth-TotalDamage,0);

	//Broadcast Damage Received
	OnDamageReceived.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal);

	//Broadcast Death
	if (CurrentHealth<=0)
	{
		OnZeroHealth_BP.Broadcast();
		OnZeroHealth_CPP.Broadcast();
	}
	
	return TotalDamage;
}

void USf_DamageController::Heal(const float AmountOfHeal, const bool bInternal)
{
	if (CurrentHealth<MaxHealth && AmountOfHeal>0 && !bInternal)
	{
		OnHeal_CPP.Broadcast();
		OnHeal_BP.Broadcast();
	}
	
	CurrentHealth = FMath::Min(CurrentHealth+AmountOfHeal,MaxHealth);
}

void USf_DamageController::RestoreHealth()
{
	CurrentHealth = MaxHealth;
}


float USf_DamageController::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USf_DamageController::GetCurrentArmor() const
{
	return CurrentArmor;
}



