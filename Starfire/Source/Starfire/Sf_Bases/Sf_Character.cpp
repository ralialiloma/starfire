// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_Character.h"

ASf_Character::ASf_Character(const FObjectInitializer& ObjectInitializer)
{
	//Equipment
	SfEquipmentComponent = CreateDefaultSubobject<USf_Equipment>(TEXT("SFEquip"));
	SfEquipmentComponent->SetUsingAbsoluteLocation(false);
	SfEquipmentComponent->SetUsingAbsoluteRotation(false);
	SfEquipmentComponent->SetUsingAbsoluteScale(false);

	//Melee Transform
	WeaponMeleeTransform = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Transform"));
	WeaponMeleeTransform->SetUsingAbsoluteLocation(false);
	WeaponMeleeTransform->SetUsingAbsoluteRotation(false);
	WeaponMeleeTransform->SetUsingAbsoluteScale(false);
	WeaponMeleeTransform->SetRelativeLocation(FVector(30,0,0));
	WeaponMeleeTransform->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponMeleeTransform->ShapeColor = FColor::Purple;
	WeaponMeleeTransform->SetLineThickness(1.0f);
	WeaponMeleeTransform->AttachToComponent(GetCapsuleComponent(),FAttachmentTransformRules::KeepRelativeTransform);

	//Damage Controller
	SfDamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("SfDamageController"));
}


void ASf_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

UAnimInstance* ASf_Character::GetCharacterAnimInstance_Implementation() const
{
	return IWeaponOwner::GetCharacterAnimInstance_Implementation();
}

FTransform ASf_Character::GetFireTransform_Implementation() const
{
	return IWeaponOwner::GetFireTransform_Implementation();
}

FMeleeInfo ASf_Character::GetMeleeInfo_Implementation() const
{
	return IWeaponOwner::GetMeleeInfo_Implementation();
}

USf_AnimHelper* ASf_Character::GetAnimDataHelper_Implementation() const
{
	return IWeaponOwner::GetAnimDataHelper_Implementation();
}


void ASf_Character::PawnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GetDebugName(this) + " has been killed!");
	OnPawnDeath_CPP.Broadcast();
	OnPawnDeath_BP.Broadcast();
}


