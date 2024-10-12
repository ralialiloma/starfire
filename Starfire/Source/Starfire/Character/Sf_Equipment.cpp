// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Equipment.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Starfire/Weapon/FireBlocks.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(EquipmentComponent, Log, All);

// Sets default values for this component's properties
USF_Equipment::USF_Equipment(): EquippedWeapon(nullptr), CurrentEquipmentFlags(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	Mobility = EComponentMobility::Type::Movable;
}

void USF_Equipment::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (!GetOwner()->Implements<UWeaponOwner>())
		UE_LOG(EquipmentComponent, Error, TEXT("Actor requires interface %s"),*UWeaponOwner::StaticClass()->GetName());

	EquippedWeapon = nullptr;
}

void USF_Equipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr)
		return;

	//Attach With New Socket
	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USF_Equipment, WeaponAttachmentSocket))
		AttachToParentMesh();

}

bool USF_Equipment::HasWeapon(AWeaponBase* WeaponBase) const
{
	if (IsValid(WeaponBase))
		return OwnedWeapons.Contains(WeaponBase);
	return false;
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

FWeaponAnimData_FP USF_Equipment::GetEquippedAnimationData_FP() const
{
	if (!IsEquipped())
		return FWeaponAnimData_FP();

	return EquippedWeapon->GetWeaponConfig().GetAnimData_FP();
}

FWeaponAnimData_TP USF_Equipment::GetEquippedAnimationData_TP() const
{
	if (!IsEquipped())
		return FWeaponAnimData_TP();

	return EquippedWeapon->GetWeaponConfig().GetAnimData_TP();
}

AWeaponBase* USF_Equipment::GetActiveWeapon() const
{
	return EquippedWeapon;
}

int USF_Equipment::GetActiveSlot() const
{
	int Slot = 0;
	GetSlotByWeapon(GetActiveWeapon(), Slot);
	return Slot;
}

void USF_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int &Slot)
{
	//If Weapon Is Already Owned
	int FoundWeaponIndex = -1;
	if (GetSlotByWeapon(WeaponToAdd,FoundWeaponIndex))
	{
		UE_LOG(EquipmentComponent, Log, TEXT("Weapon Already Equipped"));
		Slot = FoundWeaponIndex;
		return;
	}

	if (!IsValid(WeaponToAdd))
	{
		UE_LOG(EquipmentComponent, Error, TEXT("The Weapon you're trying to equip is invalid"));
		return;
	}

	//Add Weapon To List
	Slot = OwnedWeapons.Add(WeaponToAdd);
	WeaponToAdd->OnPickup(GetOwner());

	//Attach Weapon
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	WeaponToAdd->AttachToComponent(this, AttachRules, "None");

	if (Equip)
		EquipWeaponByReference(WeaponToAdd);
	else
		SetWeaponActive(WeaponToAdd, false);
}

void USF_Equipment::AddWeaponByClass(TSubclassOf<AWeaponBase> WeaponClassToAdd, bool Equip, int& Index)
{
	AWeaponBase* WeaponBase =  GetWorld()->SpawnActor<AWeaponBase>(WeaponClassToAdd);
	AddWeapon(WeaponBase, Equip, Index);
}

bool USF_Equipment::RemoveWeapon(AWeaponBase* WeaponToRemove)
{
	if (!HasWeapon(WeaponToRemove))
		return false;
	
	OwnedWeapons.Remove(WeaponToRemove);

	if (EquippedWeapon == WeaponToRemove)
		UnequipWeapon(false);
	
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	WeaponToRemove->DetachFromActor(DetachRules);
	WeaponToRemove->OnDrop();

	return true;
}

bool USF_Equipment::RemoveWeaponByClass(TSubclassOf<AWeaponBase> WeaponClassToRemove)
{
	AWeaponBase* WeaponToRemove = nullptr;
	for (auto Weapon : OwnedWeapons)
	{
		if (!IsValid(Weapon))
			continue;

		if (Weapon->GetClass() == WeaponClassToRemove)
		{
			WeaponToRemove = Weapon;
			break;
		}
	}

	return RemoveWeapon(WeaponToRemove);
}

void USF_Equipment::CycleWeapons(ENavigationDirectionType Direction)
{
	if (OwnedWeapons.Num() <= 0)
		return;
	
	int CurrentSlot = INDEX_NONE;
	int NextSlot = INDEX_NONE;
	if (GetSlotByWeapon(EquippedWeapon, CurrentSlot))
	{
		switch (Direction) {
			case ENavigationDirectionType::Next:
				NextSlot = (CurrentSlot + 1) % OwnedWeapons.Num();
				break;
			case ENavigationDirectionType::Previous:
				NextSlot = (CurrentSlot - 1 + OwnedWeapons.Num()) % OwnedWeapons.Num();
				break;
			}
	}
	else
	{
		NextSlot = 0;
	}

	EquipWeaponBySlot(NextSlot);
}

bool USF_Equipment::EquipWeaponByReference(AWeaponBase* Weapon)
{
	return ActivateWeapon(Weapon);
}

bool USF_Equipment::EquipWeaponBySlot(int Slot)
{
	AWeaponBase* Weapon = nullptr;
	if (GetWeaponBySlot(Slot, Weapon))
		return ActivateWeapon(Weapon);

	UE_LOG(EquipmentComponent, Error, TEXT("The weapon slot you're trying to activate is invalid"));
	return false;
}

bool USF_Equipment::UnequipWeapon(bool HideWeapon)
{
	if (!IsValid(EquippedWeapon))
		return false;

	AWeaponBase* OldWeapon = EquippedWeapon;
	EquippedWeapon->OnUnequip();
	if (HideWeapon)
		SetWeaponActive(EquippedWeapon, false);
	
	EquippedWeapon = nullptr;
	OnWeaponChange.Broadcast(nullptr, OldWeapon);

	return true;
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

bool USF_Equipment::Reload() const
{
	if (!IsEquipped())
		return false;

	float MontageTime = 0;
	return EquippedWeapon->Reload(MontageTime);
}


bool USF_Equipment::Reload(float &OutMontageTime)
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Reload(OutMontageTime);
}

void USF_Equipment::StopReloading()
{
	if (!IsEquipped())
		return;
	
	EquippedWeapon->StopReloading();
}

bool USF_Equipment::IsReloading() const
{
	if (!IsEquipped())
		return false;

	return  EquippedWeapon->IsReloading();
}

bool USF_Equipment::IsOnMeleeCooldown() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->IsOnMeleeCooldown();
}

bool USF_Equipment::Aim()
{
	if (!IsEquipped())
		return false;

	EquippedWeapon->AimDownSight();
	return true;
}

void USF_Equipment::StopAiming()
{
	if (!IsEquipped())
		return;
	
	EquippedWeapon->StopAiming();
}

bool USF_Equipment::IsOnFireCooldown() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsOnFireCooldown();
}

bool USF_Equipment::Melee()
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Melee();
}


// Called every frame
void USF_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int NewFlags = GetCompressedFlags();
	if (NewFlags != CurrentEquipmentFlags)
		OnEquipmentFlagsChange.Broadcast(CurrentEquipmentFlags, NewFlags);
	CurrentEquipmentFlags = NewFlags;
}


bool USF_Equipment::CanMelee() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->CanMelee();
}

FWeaponConfig USF_Equipment::GetWeaponConfig()
{
	if (!IsEquipped())
	{
		UE_LOG(EquipmentComponent, Error, TEXT("Cannot get config because no weapon is equipped"))
		return FWeaponConfig();
	}

	return  GetActiveWeapon()->GetWeaponConfig();
}


int USF_Equipment::GetCompressedFlags() const
{
	int EquipmentFlags = 0;

	USf_FunctionLibrary::SetBit(IsEquipped(),EquipmentFlags,EEquipmentFlags::EquipmentState_Equipped);
	USf_FunctionLibrary::SetBit(IsOnFireCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_FireCooldown);
	USf_FunctionLibrary::SetBit(IsAiming(),EquipmentFlags,EEquipmentFlags::EquipmentState_Aiming);
	USf_FunctionLibrary::SetBit(IsReloading(),EquipmentFlags,EEquipmentFlags::EquipmentState_Reloading);
	USf_FunctionLibrary::SetBit(IsOnMeleeCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_MeleeCooldown);

	return EquipmentFlags;
}

bool USF_Equipment::CheckFlag(EEquipmentFlags EquipmentFlag) const
{
	return USf_FunctionLibrary::CheckBitFlag(GetCompressedFlags(),EquipmentFlag);
}

bool USF_Equipment::CheckFlagForState(EEquipmentFlags EquipmentFlag, int StateToCheck) const
{
	return USf_FunctionLibrary::CheckBitFlag(StateToCheck,EquipmentFlag);
}

bool USF_Equipment::GetSlotByWeapon(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = INDEX_NONE;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex = OwnedWeapons.Find(WeaponBase);
	return HasWeapon(WeaponBase);
}

bool USF_Equipment::GetWeaponBySlot(int Index, AWeaponBase*& OutWeaponBase) const
{
	OutWeaponBase = nullptr;
	if (Index < 0 || Index >= OwnedWeapons.Num())
		return false;

	OutWeaponBase = OwnedWeapons[Index];
	return true;
}

void USF_Equipment::SetWeaponActive(AWeaponBase* Weapon, bool Active)
{
	if (!HasWeapon(Weapon))
		return;

	Weapon->SetWeaponActive(Active);
}

bool USF_Equipment::ActivateWeapon(AWeaponBase* Weapon)
{
	if (!HasWeapon(Weapon))
		return false;

	AWeaponBase* OldWeapon = EquippedWeapon;
	if (OldWeapon)
	{
		OldWeapon->OnUnequip();
		SetWeaponActive(OldWeapon, false);
	}
	
	EquippedWeapon = Weapon;
	EquippedWeapon->OnEquip();
	SetWeaponActive(EquippedWeapon, true);

	OnWeaponChange.Broadcast(EquippedWeapon, OldWeapon);
	return true;
}

void USF_Equipment::AttachToParentMesh()
{
	USceneComponent* Parent =  GetAttachParent();
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	AttachToComponent(Parent,AttachRules,WeaponAttachmentSocket);
}

TArray<FName> USF_Equipment::GetWeaponAttachmentSocketOptions() const
{
	USceneComponent* Parent =  GetAttachParent();
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Parent);
	if (!IsValid(SkeletalMeshComponent))
		return TArray<FName>{"None"};

	USkeletalMesh* MeshAsset =  SkeletalMeshComponent->GetSkeletalMeshAsset();
	if (!IsValid(MeshAsset))
		return TArray<FName>{"None"};
	
	TArray<FName> AllSocketNames{};
	const TArray<USkeletalMeshSocket*> AllSockets = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetActiveSocketList();

	for (int32 SocketIdx = 0; SocketIdx < AllSockets.Num(); ++SocketIdx)
		AllSocketNames.Add(AllSockets[SocketIdx]->SocketName);
	
	return AllSocketNames;
}

