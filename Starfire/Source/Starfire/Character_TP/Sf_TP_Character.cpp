// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_TP_Character.h"

#include "NavigationSystem.h"
#include "NavModifierComponent.h"
#include "EQS/TetherPointSystem/Sf_TetherPointGen.h"
#include "Features/Combat/CF_Combat_Config.h"
#include "Features/Cover/CF_Cover_Config.h"
#include "Features/Death/CF_Death_Config.h"
#include "Features/DynamicMoveTarget/CF_DynamicMoveTarget_Config.h"
#include "Features/Locomotion/CF_Locomotion_Config.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
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

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		//User defined features
		for (USf_CharacterFeature_Config* Config: FeatureConfigs)
			TryAddFeature(Config);

		//Necessary FeaturesNew
		for (TSubclassOf<USf_CharacterFeature_Config> ConfigType: GetAllStartConfigs())
		{
			TryAddFeatureByConfigClass(ConfigType);
		}
	}
}

	


USf_CharacterFeature* ASf_TP_Character::GetFeatureByClass(const TSubclassOf<USf_CharacterFeature> Class)
{
	if(!IsValid(Class))
		return nullptr;
	
	for (USf_CharacterFeature* Element : FeaturesNew)
	{
		if (!IsValid(Element))
			continue;
		
		if (Element->GetClass()->IsChildOf(Class))
		{
			return Element;
		}
	}

	return nullptr;
}

bool ASf_TP_Character::TryAddFeature(const USf_CharacterFeature_Config* FeatureConfig)
{
	if (!IsValid(FeatureConfig))
	{
		return false;
	}
	
	const TSubclassOf<USf_CharacterFeature> NewFeatureType{FeatureConfig->GetFeatureType()};
	
	if (!IsValid(FeatureConfig)|| NewFeatureType->HasAnyClassFlags(CLASS_Abstract))
	{
		return false;
	}
	
	for (auto Element : FeaturesNew)
	{
		if (!IsValid(Element))
			continue;
		
		if (Element->GetClass()->IsChildOf(NewFeatureType) || NewFeatureType->IsChildOf(Element->GetClass()))
		{
			return false;
		}
	}
	
	USf_CharacterFeature* Feature = FeatureConfig->MakeFeature(this);
	FeaturesNew.Add(Feature);
	UE_LOG(LogTemp, Log, TEXT("Added Feature %s"),*FeatureConfig->GetName())
	return true;
}

bool ASf_TP_Character::TryAddFeatureByConfigClass(const TSubclassOf<USf_CharacterFeature_Config> FeatureConfigType)
{
	if (!IsValid(FeatureConfigType)|| FeatureConfigType->HasAnyClassFlags(CLASS_Abstract))
	{
		return false;
	}
	
	for (auto Element : FeatureConfigs)
	{
		if (!IsValid(Element))
			continue;
		
		if (Element->GetClass()->IsChildOf(FeatureConfigType) || FeatureConfigType->IsChildOf(Element->GetClass()))
		{
			return false;
		}
	}
	USf_CharacterFeature_Config* NewConfig = NewObject<USf_CharacterFeature_Config>(this,FeatureConfigType);
	FeatureConfigs.Add(NewConfig);
	return TryAddFeature(NewConfig);
}


USf_Equipment* ASf_TP_Character::GetSfEquipment()
{
	return SfEquipmentComponent;
}


USf_TP_CharacterMovementComponent* ASf_TP_Character::GetSf_TP_CharacterMovement()
{
	return SFCharacterMovementComponent;
}

void ASf_TP_Character::BeginPlay()
{
	Super::BeginPlay();
	
	for (USf_CharacterFeature* Feature: FeaturesNew)
		Feature->OnBeginPlay();


	ASf_TetherPointGen* TetherPointGen =  ASf_TetherPointGen::Get(GetWorld());
	if (IsValid(TetherPointGen))
		TetherPointGen->RegisterActor(this);
}

void ASf_TP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (USf_CharacterFeature* Feature: FeaturesNew)
		Feature->OnTick(DeltaTime);

}

// Called to bind functionality to input
void ASf_TP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
TSet<TSubclassOf<USf_CharacterFeature_Config>> ASf_TP_Character::GetAllStartConfigs()
{
	TSet<TSubclassOf<USf_CharacterFeature_Config>> AllFeatures{
		UCF_Cover_Config::StaticClass(),
		UCF_Combat_Config::StaticClass(),
		UCF_Locomotion_Config::StaticClass(),
		UCF_Death_Config::StaticClass(),
		UCF_DynamicMoveTarget_Config::StaticClass()};
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
	/*const FVector Location = WeaponMeleeTransform->GetComponentLocation();
	const FVector Extent = WeaponMeleeTransform->GetScaledBoxExtent();
	const FRotator Rotation = WeaponMeleeTransform->GetComponentRotation();
	const FVector Direction = Rotation.Vector();*/
	const FMeleeInfo MeleeInfo = FMeleeInfo(WeaponMeleeTransform);
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



