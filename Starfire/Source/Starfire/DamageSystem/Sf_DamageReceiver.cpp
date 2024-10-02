// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_DamageReceiver.h"

USf_DamageReceiver::USf_DamageReceiver()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;
	MaxArmor = 1;
}


void USf_DamageReceiver::BeginPlay()
{
	Super::BeginPlay();
}

void USf_DamageReceiver::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentHealth = MaxHealth;
	CurrentArmor = MaxArmor;
}

void USf_DamageReceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


float USf_DamageReceiver::ApplyDamage(float Damage,FVector HitLocation, FVector HitNormal,UPrimitiveComponent* HitComponent)
{
	if (CurrentHealth<=0)
		return 0;
	
	//Get Hitbox
	const USf_Hitbox* Hitbox =  Cast<USf_Hitbox>(HitComponent);
	if (!IsValid(Hitbox))
		return 0;

	//Total Damage
	float TotalDamage = (Damage/CurrentArmor)*Hitbox->DamageMultiplier;

	//Update CurrentHealth
	CurrentHealth = FMath::Max(CurrentHealth-TotalDamage,0);

	//Broadcast Damage Received
	OnDamageReceived.Broadcast(CurrentHealth,TotalDamage,HitLocation,HitNormal);

	//Broadcast Death
	if (CurrentHealth<=0)
		OnZeroHealth.Broadcast();

	return TotalDamage;
}


float USf_DamageReceiver::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USf_DamageReceiver::GetCurrentArmor() const
{
	return CurrentArmor;
}



