// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Equipment.h"
#include "Project/Weapon/FireBlocks.h"
#include "Project/Weapon/WeaponOwner.h"
#include "Project/Utility/InputSignalType.h"

DEFINE_LOG_CATEGORY_STATIC(EquipmentComponent, Log, All);

// Sets default values for this component's properties
USF_Equipment::USF_Equipment(): EquippedWeapon(nullptr), WeaponOwner(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	Mobility = EComponentMobility::Type::Movable;
}

void USF_Equipment::InitializeComponent()
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
void USF_Equipment::BeginPlay()
{
	Super::BeginPlay();
}

bool USF_Equipment::IsEquipped() const
{
	return IsValid(EquippedWeapon);
}

bool USF_Equipment::IsAiming() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsAiming();
}

FWeaponAnimData USF_Equipment::GetAnimationData() const
{
	if (!IsEquipped())
		return FWeaponAnimData();

	return EquippedWeapon->GetActiveConfig().GetAnimData();
}

void USF_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int &Index)
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

	if (Equip)
	{
		EquippedWeapon = WeaponToAdd;
		EquippedWeapon->OnEquip(GetOwner());
	}

	UE_LOG(EquipmentComponent, Log, TEXT("Equipped Weapon: %s"),*EquippedWeapon->GetName());

	//todo SetWeaponActive
	//todo call pickup event
	
}

void USF_Equipment::AddWeaponByClass(TSubclassOf<AWeaponBase> ActorClass, bool Equip, int& Index)
{
	AActor* SpawnedActor =  GetWorld()->SpawnActor(ActorClass);
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(SpawnedActor);
	AddWeapon(WeaponBase,Equip,Index);
}

bool USF_Equipment::Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock)
{
	if (!IsEquipped())
	{
		OutFireBlock = EFireBlock::NoWeapon;
		return false;
	}
	
	return EquippedWeapon->Fire(InputSignal,FireType,OutHitResult,OutFireBlock);
}



bool USF_Equipment::CanReload() const
{
	if (!IsEquipped())
	{
		return false;
	}
	return true;
}

bool USF_Equipment::Reload()
{
	if (!CanReload())
	{
		UE_LOG(EquipmentComponent, Log, TEXT("Cannot run Reload"))
	}

	return EquippedWeapon->Reload();
}

void USF_Equipment::StopReloading()
{
	if (!IsEquipped())
		return;

	return EquippedWeapon->StopReloading();
}


// Called every frame
void USF_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}




bool USF_Equipment::GetSlot(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = -1;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex=  OwnedWeapons.Find(WeaponBase);
	return OwnedWeapons.Contains(WeaponBase);
}

