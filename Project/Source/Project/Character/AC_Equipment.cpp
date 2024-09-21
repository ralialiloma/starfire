// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Equipment.h"

#include "Project/Weapon/FireBlocks.h"
#include "Project/Weapon/WeaponOwner.h"


// Sets default values for this component's properties
UAC_Equipment::UAC_Equipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_Equipment::BeginPlay()
{
	Super::BeginPlay();

	WeaponOwner = GetOwner();
	if (!WeaponOwner->Implements<UWeaponOwner>())
		UE_LOG(LogTemp,
		Error,
		TEXT("Actor requires interface %s "),
		*UWeaponOwner::StaticClass()->GetName())
}

bool UAC_Equipment::GetIsEquipped() const
{
	return IsValid(EquippedWeapon);
}

void UAC_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, bool Equip, int &Index)
{
	//If Weapon Is Already Equipped
	int FoundWeaponIndex = -1;
	if (GetSlot(WeaponToAdd,FoundWeaponIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Already Equipped"))
		Index = FoundWeaponIndex;
		return;
	}

	//Add Weapon To List
	OwnedWeapons.Add(WeaponToAdd);
	Index =  OwnedWeapons.Find(WeaponToAdd);

	//Attach Weapon
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	WeaponToAdd->AttachToComponent(this, AttachRules, "None");
	WeaponToAdd->SetOwner(this->GetOwner());

	if (Equip)
		EquippedWeapon = WeaponToAdd;

	//todo SetWeaponActive
	//todo call pickup event
	
}

bool UAC_Equipment::Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock)
{
	if (!IsValid(EquippedWeapon))
	{
		OutFireBlock = EFireBlock::NoWeapon;
		return false;
	}

	FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(WeaponOwner);
	
	return EquippedWeapon->Fire(InputSignal,FireType,OutHitResult,OutFireBlock);
}


// Called every frame
void UAC_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAC_Equipment::GetSlot(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = -1;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex=  OwnedWeapons.Find(WeaponBase);
	return OwnedWeapons.Contains(WeaponBase);
}

