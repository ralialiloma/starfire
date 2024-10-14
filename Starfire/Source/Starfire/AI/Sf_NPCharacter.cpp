// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_NPCharacter.h"

#include "WeaponAnimMontageController_TP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Features/EF_Cover.h"
#include "Features/EF_Combat.h"
#include "Features/EF_Death.h"
#include "Features/EF_Locomotion.h"
#include "Perception/AIPerceptionComponent.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/DamageSystem/Sf_DamageController.h"

ASf_NPCharacter::ASf_NPCharacter(const FObjectInitializer& ObjectInitializer): Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USf_TP_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

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

	//WeaponAnimMontageController
	WeaponAnimMontageController = CreateDefaultSubobject<USf_WeaponAnimMontageController_TP>(TEXT("WeaponAnimMontageController"));

	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));

	//Movement Component
	SFCharacterMovementComponent = Cast<USf_TP_CharacterMovementComponent>(GetCharacterMovement());;

	//Pawn Sensing
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	//Pawn Sensing
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void ASf_NPCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
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
	UE_LOG(LogTemp, Log, TEXT("Added Feature %s"),*FeatureType->GetName())
	return true;
}

USf_DamageController* ASf_NPCharacter::GetSfDamageController()
{
	return DamageController;
}

USF_Equipment* ASf_NPCharacter::GetSfEquipment()
{
	return SfEquipmentComponent;
}

USf_TP_CharacterMovementComponent* ASf_NPCharacter::GetSfMovement()
{
	return SFCharacterMovementComponent;
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
	for (TSubclassOf<UEnemyFeature> Feature: GetAllStartFeatures())
	{
		TryAddFeature(Feature);
	}
}

TSet<TSubclassOf<UEnemyFeature>> ASf_NPCharacter::GetAllStartFeatures() const
{
	TSet<TSubclassOf<UEnemyFeature>> AllFeatures{
		UEF_Cover::StaticClass(),
		UEF_Combat::StaticClass(),
		UEF_Locomotion::StaticClass(),
		UEF_Death::StaticClass()};
	AllFeatures.Append(StartFeatures);
	return AllFeatures;
}

UAnimInstance* ASf_NPCharacter::GetCharacterAnimInstance_Implementation() const
{
	return GetMesh()->GetAnimInstance();
}

USf_WeaponAnimMontageController* ASf_NPCharacter::GetAnimMontageController_Implementation() const
{
	return WeaponAnimMontageController;
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

FCollisionQueryParams ASf_NPCharacter::GetIgnoreCharacterParams()
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetSfEquipment()->GetActiveWeapon());

	return Params;
}

TArray<AActor*> ASf_NPCharacter::GetIgnoreActors()
{
	TArray<AActor*> IgnoreActors{this};
	AWeaponBase* ActiveWapon = GetSfEquipment()->GetActiveWeapon();
	if (IsValid(ActiveWapon))
		IgnoreActors.Add(ActiveWapon);
	return IgnoreActors;
}

