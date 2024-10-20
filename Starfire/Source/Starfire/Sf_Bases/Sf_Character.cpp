// Fill out your copyright notice in the Description page of Project Settings.
#include "Starfire/Character_TP/Sf_TP_Character.h"

ASf_Character::ASf_Character(const FObjectInitializer& ObjectInitializer)
{
	//Equipment
	SfEquipmentComponent = CreateDefaultSubobject<USf_Equipment>(TEXT("SFEquip"));
	SfEquipmentComponent->SetUsingAbsoluteLocation(false);
	SfEquipmentComponent->SetUsingAbsoluteRotation(false);
	SfEquipmentComponent->SetUsingAbsoluteScale(false);

	//Melee Transform
	MeleeTransform = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Transform"));
	MeleeTransform->SetUsingAbsoluteLocation(false);
	MeleeTransform->SetUsingAbsoluteRotation(false);
	MeleeTransform->SetUsingAbsoluteScale(false);
	MeleeTransform->SetRelativeLocation(FVector(30,0,0));
	MeleeTransform->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeleeTransform->ShapeColor = FColor::Purple;
	MeleeTransform->SetLineThickness(1.0f);

	//Damage Controller
	SfDamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("SfDamageController"));
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

USf_WeaponAnimMontageController* ASf_Character::GetAnimMontageController_Implementation() const
{
	return IWeaponOwner::GetAnimMontageController_Implementation();
}

void ASf_Character::PawnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GetDebugName(this) + " has been killed!");
	OnPawnDeath_CPP.Broadcast();
	OnPawnDeath_BP.Broadcast();
}


