// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_NPCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/DamageSystem/Sf_DamageReceiver.h"

ASf_NPCharacter::ASf_NPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SfDamageReceiver = CreateDefaultSubobject<USf_DamageReceiver>(TEXT("Damage Receiver"));

	//Equipment
	SfEquipmentComponent = CreateDefaultSubobject<USF_Equipment>(TEXT("SFEquip"));
	SfEquipmentComponent->SetUsingAbsoluteLocation(false);
	SfEquipmentComponent->SetUsingAbsoluteRotation(false);
	SfEquipmentComponent->SetUsingAbsoluteScale(false);
	SfEquipmentComponent->SetupAttachment(GetMesh(),TEXT("GripPoint"));

	//Melee Transform
	MeleeTransform = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Transform"));
	MeleeTransform->SetUsingAbsoluteLocation(false);
	MeleeTransform->SetUsingAbsoluteRotation(false);
	MeleeTransform->SetUsingAbsoluteScale(false);
	MeleeTransform->SetRelativeLocation(FVector(30,0,0));
	MeleeTransform->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeleeTransform->ShapeColor = FColor::Purple;
	MeleeTransform->SetLineThickness(1.0f);
}

void ASf_NPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ImportStartFeatures();
}

UEnemyFeature* ASf_NPCharacter::GetFeatureByClass(TSubclassOf<UEnemyFeature> Class)
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

bool ASf_NPCharacter::TryAddFeature(TSubclassOf<UEnemyFeature>&  FeatureType)
{
	if (!FeatureType)
	{
		return false;
	}

	for (auto Element : Features)
	{
		if (Element->GetClass() == FeatureType)
		{
			return false;
		}
	}
	
	UEnemyFeature* Feature = NewObject<UEnemyFeature>(this,FeatureType);
	Features.Add(Feature);
	Feature->Initialize(this);
	return true;
}

USf_DamageReceiver* ASf_NPCharacter::GetSfDamageReceiver()
{
	return SfDamageReceiver;
}

USF_Equipment* ASf_NPCharacter::GetSfEquipment()
{
	return SfEquipmentComponent;
}


// Called when the game starts or when spawned
void ASf_NPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASf_NPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASf_NPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASf_NPCharacter::ImportStartFeatures()
{
	for (TSubclassOf<UEnemyFeature> Feature: StartFeatures)
	{
		TryAddFeature(Feature);
	}
}

UAnimInstance* ASf_NPCharacter::GetCharacterAnimInstance_Implementation() const
{
	return GetMesh()->GetAnimInstance();
}

FTransform ASf_NPCharacter::GetFireTransform_Implementation() const
{
	if (SfEquipmentComponent->IsEquipped())
		return SfEquipmentComponent->GetActiveWeapon()->GetMuzzleTransform();
	
	return FTransform::Identity;
}

FMeleeInfo ASf_NPCharacter::GetMeleeInfo_Implementation() const
{
	const FVector Location = MeleeTransform->GetComponentLocation();
	const FVector Extent = MeleeTransform->GetScaledBoxExtent();
	const FRotator Rotation = MeleeTransform->GetComponentRotation();
	const FVector Direction = GetActorLocation()-Location;
	const FMeleeInfo MeleeInfo = FMeleeInfo(Location,Extent,Rotation,Direction);
	return MeleeInfo;
}

