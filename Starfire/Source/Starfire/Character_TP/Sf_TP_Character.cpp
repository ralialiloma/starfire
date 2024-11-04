// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_TP_Character.h"
#include "Features/CF_Cover.h"
#include "Features/CF_Combat.h"
#include "Features/CF_Death.h"
#include "Features/CF_Locomotion.h"
#include "Perception/AIPerceptionComponent.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"

ASf_TP_Character::ASf_TP_Character(const FObjectInitializer& ObjectInitializer): Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USf_TP_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	//Equipment
	SfEquipmentComponent->SetupAttachment(GetMesh(),TEXT("GripPoint"));
	
	//WeaponAnimMontageController
	WeaponAnimMontageController = CreateDefaultSubobject<USf_WeaponAnimMontageController_TP>(TEXT("WeaponAnimMontageController"));
	
	//Movement Component
	SFCharacterMovementComponent = Cast<USf_TP_CharacterMovementComponent>(GetCharacterMovement());;
}

void ASf_TP_Character::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ASf_TP_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ImportStartFeatures();
}

USf_CharacterFeature* ASf_TP_Character::GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class)
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

bool ASf_TP_Character::TryAddFeature(TSubclassOf<USf_CharacterFeature>&  FeatureType)
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
	
	USf_CharacterFeature* Feature = NewObject<USf_CharacterFeature>(this,FeatureType);
	Features.Add(Feature);
	Feature->Initialize(this);
	UE_LOG(LogTemp, Log, TEXT("Added Feature %s"),*FeatureType->GetName())
	return true;
}


USf_Equipment* ASf_TP_Character::GetSfEquipment()
{
	return SfEquipmentComponent;
}

USf_TP_CharacterMovementComponent* ASf_TP_Character::GetSf_TP_CharacterMovement()
{
	return SFCharacterMovementComponent;
}




// Called when the game starts or when spawned
void ASf_TP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASf_TP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASf_TP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASf_TP_Character::ImportStartFeatures()
{
	for (TSubclassOf<USf_CharacterFeature> Feature: GetAllStartFeatures())
	{
		TryAddFeature(Feature);
	}
}

TSet<TSubclassOf<USf_CharacterFeature>> ASf_TP_Character::GetAllStartFeatures() const
{
	TSet<TSubclassOf<USf_CharacterFeature>> AllFeatures{
		UCF_Cover::StaticClass(),
		UCF_Combat::StaticClass(),
		UCF_Locomotion::StaticClass(),
		UCF_Death::StaticClass()};
	AllFeatures.Append(StartFeatures);
	return AllFeatures;
}

UAnimInstance* ASf_TP_Character::GetCharacterAnimInstance_Implementation() const
{
	return GetMesh()->GetAnimInstance();
}

USf_AnimHelper* ASf_TP_Character::GetAnimDataHelper_Implementation() const
{
	return WeaponAnimMontageController;
}

FTransform ASf_TP_Character::GetFireTransform_Implementation() const
{
	//if (SfEquipmentComponent->IsEquipped())
	//	return SfEquipmentComponent->GetActiveWeapon()->GetMuzzleTransform();

	return FTransform(GetControlRotation(),GetActorLocation()+FVector(0,0,30),GetActorScale());
	
	//return FTransform::Identity;
}

FMeleeInfo ASf_TP_Character::GetMeleeInfo_Implementation() const
{
	const FVector Location = MeleeTransform->GetComponentLocation();
	const FVector Extent = MeleeTransform->GetScaledBoxExtent();
	const FRotator Rotation = MeleeTransform->GetComponentRotation();
	const FVector Direction = GetActorLocation()-Location;
	const FMeleeInfo MeleeInfo = FMeleeInfo(Location,Extent,Rotation,Direction);
	return MeleeInfo;
}

FCollisionQueryParams ASf_TP_Character::GetIgnoreCharacterParams()
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetSfEquipment()->GetActiveWeapon());

	return Params;
}

TArray<AActor*> ASf_TP_Character::GetIgnoreActors()
{
	TArray<AActor*> IgnoreActors{this};
	AWeaponBase* ActiveWapon = GetSfEquipment()->GetActiveWeapon();
	if (IsValid(ActiveWapon))
		IgnoreActors.Add(ActiveWapon);
	return IgnoreActors;
}

