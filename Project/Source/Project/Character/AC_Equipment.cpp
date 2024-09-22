// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Equipment.h"
#include "Project/Weapon/FireBlocks.h"
#include "Project/Weapon/WeaponOwner.h"
#include "Project/Utility/InputSignalType.h"

DEFINE_LOG_CATEGORY_STATIC(EquipmentComponent, Display, Display);

// Sets default values for this component's properties
UAC_Equipment::UAC_Equipment(): EquippedWeapon(nullptr), WeaponOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	Mobility = EComponentMobility::Type::Movable;
}

void UAC_Equipment::InitializeComponent()
{
	Super::InitializeComponent();

	WeaponOwner = GetOwner();
	if (!WeaponOwner->Implements<UWeaponOwner>())
		UE_LOG(EquipmentComponent,
		Error,
		TEXT("Actor requires interface %s "),
		*UWeaponOwner::StaticClass()->GetName())

	EquippedWeapon = nullptr;
}

// Called when the game starts
void UAC_Equipment::BeginPlay()
{
	Super::BeginPlay();
}

bool UAC_Equipment::IsEquipped() const
{
	return IsValid(EquippedWeapon);
}

bool UAC_Equipment::IsAiming() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsAiming();
}

FWeaponAnimData UAC_Equipment::GetAnimationData() const
{
	if (!IsEquipped())
		return FWeaponAnimData();

	return EquippedWeapon->GetActiveConfig().GetAnimData();
}

void UAC_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int &Index)
{
	//If Weapon Is Already Equipped
	int FoundWeaponIndex = -1;
	if (GetSlot(WeaponToAdd,FoundWeaponIndex))
	{
		UE_LOG(EquipmentComponent, Log, TEXT("Weapon Already Equipped"));
		Index = FoundWeaponIndex;
		return;
	}

	if (!IsValid(WeaponToAdd))
	{
		UE_LOG(EquipmentComponent, Error, TEXT("The Weapon you're trying to equip is invalid"));
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

	//UE_LOG(EquipmentComponent, Log, TEXT("Equipped Weapon: ",Sta));
	

	if (Equip)
	{
		EquippedWeapon = WeaponToAdd;
		EquippedWeapon->OnEquip(GetOwner());
	}

	//todo SetWeaponActive
	//todo call pickup event
	
}

bool UAC_Equipment::Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock)
{
	UE_LOG(EquipmentComponent, Log, TEXT("Playing Primary Fire"))
	
	if (!IsEquipped())
	{
		OutFireBlock = EFireBlock::NoWeapon;
		return false;
	}
	
	return EquippedWeapon->Fire(InputSignal,FireType,OutHitResult,OutFireBlock);
}


// Called every frame
void UAC_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}




bool UAC_Equipment::GetSlot(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = -1;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex=  OwnedWeapons.Find(WeaponBase);
	return OwnedWeapons.Contains(WeaponBase);
}

