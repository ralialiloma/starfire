// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

#include "Starfire/DamageSystem/Sf_DamageReceiver.h"

AEnemyAI::AEnemyAI()
{
	PrimaryActorTick.bCanEverTick = true;
	SfDamageReceiver = CreateDefaultSubobject<USf_DamageReceiver>(TEXT("Damage Receiver"));
}

UEnemyFeature* AEnemyAI::GetFeatureByClass(TSubclassOf<UEnemyFeature> Class)
{
	for (auto Element : Features)
	{
		if (Element->GetClass() == Class)
		{
			return Element;
		}
	}

	return nullptr;
}

bool AEnemyAI::TryAddFeature(UEnemyFeature* Feature)
{
	for (auto Element : Features)
	{
		if (Element->GetClass() == Feature->GetClass())
		{
			return false;
		}
	}

	Features.Add(Feature);
	Feature->Initialize(this);
	
	return true;
}

USf_DamageReceiver* AEnemyAI::GetSfDamageReceiver()
{
	return SfDamageReceiver;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

