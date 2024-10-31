// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Equipment.h"

#include "Engine/SkeletalMeshSocket.h"

#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(EquipmentComponent, Log, All);

// Sets default values for this component's properties
USf_Equipment::USf_Equipment(): EquippedWeapon(nullptr), CurrentEquipmentFlags(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	Mobility = EComponentMobility::Type::Movable;
}

void USf_Equipment::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (!GetOwner()->Implements<UWeaponOwner>())
		UE_LOG(EquipmentComponent, Error, TEXT("Actor requires interface %s"),*UWeaponOwner::StaticClass()->GetName());

	EquippedWeapon = nullptr;
}

#if WITH_EDITOR
void USf_Equipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr)
		return;

	//Attach With New Socket
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USf_Equipment, WeaponAttachmentSocket))
		AttachToParentMesh();

}
#endif


bool USf_Equipment::HasWeapon(AWeaponBase* WeaponBase) const
{
	if (IsValid(WeaponBase))
		return OwnedWeapons.Contains(WeaponBase);
	return false;
}

// Called when the game starts
void USf_Equipment::BeginPlay()
{
	Super::BeginPlay();
}

bool USf_Equipment::IsEquipped() const
{
	return IsValid(EquippedWeapon);
}

bool USf_Equipment::IsAiming() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsAiming();
}

FWeaponAnimData_FP USf_Equipment::GetEquippedAnimationData_FP() const
{
	if (!IsEquipped())
		return FWeaponAnimData_FP();

	return EquippedWeapon->GetWeaponConfig().GetAnimData_FP();
}

FWeaponAnimData_TP USf_Equipment::GetEquippedAnimationData_TP() const
{
	if (!IsEquipped())
		return FWeaponAnimData_TP();

	return EquippedWeapon->GetWeaponConfig().GetAnimData_TP();
}

AWeaponBase* USf_Equipment::GetActiveWeapon() const
{
	return EquippedWeapon;
}

AWeaponBase* USf_Equipment::GetWeaponByClass(TSubclassOf<AWeaponBase> WeaponClass)
{
	AWeaponBase** FoundWeapon =  OwnedWeapons.FindByPredicate([WeaponClass](AWeaponBase* Weapon)->bool
	{
		return Weapon->GetClass() == WeaponClass;
	} );

	if (FoundWeapon==nullptr)
	{
		return nullptr;
	}
	else
	{
		return *FoundWeapon;
	}
	
}

int USf_Equipment::GetActiveSlot() const
{
	int Slot = 0;
	GetSlotByWeapon(GetActiveWeapon(), Slot);
	return Slot;
}

void USf_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int &Slot)
{
	if (!CanAddWeapon(WeaponToAdd))
		return;
	
	//If Weapon Is Already Owned
	int FoundWeaponIndex = -1;
	if (GetSlotByWeapon(WeaponToAdd,FoundWeaponIndex))
	{
		UE_LOG(EquipmentComponent, Log, TEXT("Weapon Already Equipped"));
		Slot = FoundWeaponIndex;
		return;
	}

	//Add Weapon To List
	Slot = OwnedWeapons.Add(WeaponToAdd);
	WeaponToAdd->SetNewHolder(this);
	WeaponToAdd->OnPickup(this);

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
		SetWeaponActive(WeaponToAdd, EquippedWeapon, false);
}

void USf_Equipment::AddWeaponByClass(const TSubclassOf<AWeaponBase> WeaponClassToAdd, const bool Equip, int& Index)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(EquipmentComponent, Error, TEXT("GetWorld returned nullptr"));
		return;
	}
	
	if (!IsValid(WeaponClassToAdd)||!IsValid(WeaponClassToAdd.Get()))
	{
		UE_LOG(EquipmentComponent, Error, TEXT("Invalid Weapon Class To Add"))
		return;
	}

	AWeaponBase* WeaponBase =  World->SpawnActor<AWeaponBase>(WeaponClassToAdd);
	AddWeapon(WeaponBase, Equip, Index);
}

bool USf_Equipment::CanAddWeapon(AWeaponBase* WeaponToAdd) const
{
	if (!CanAddWeapons())
	{
		return false;
	}
	
	if (!IsValid(WeaponToAdd))
	{
		UE_LOG(EquipmentComponent, Error, TEXT("The Weapon you're trying to equip is invalid"));
		return false;
	}
	return true;
}

bool USf_Equipment::CanAddWeapons() const
{
	return IS_ACTION_ALLOWED(EquippedWeapon,AddWeapon);
}

bool USf_Equipment::CanRemoveWeapon(AWeaponBase* WeaponToRemove) const
{
	if (!CanRemoveWeapons())
		return false;
	
	if (!HasWeapon(WeaponToRemove))
		return false;

	return true;
}

bool USf_Equipment::CanRemoveWeapons() const
{
	return IS_ACTION_ALLOWED(EquippedWeapon,RemoveWeapon);
}


bool USf_Equipment::RemoveWeapon(AWeaponBase* WeaponToRemove)
{
	if (!CanRemoveWeapon(WeaponToRemove))
		return false;
	
	OwnedWeapons.Remove(WeaponToRemove);

	if (EquippedWeapon == WeaponToRemove)
		UnequipWeapon(false);
	
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	WeaponToRemove->DetachFromActor(DetachRules);
	WeaponToRemove->OnDrop();

	return true;
}

bool USf_Equipment::RemoveWeaponActiveWeapon()
{
	if (!IsEquipped())
		return false;

	return  RemoveWeapon(GetActiveWeapon());
}

bool USf_Equipment::RemoveWeaponByClass(TSubclassOf<AWeaponBase> WeaponClassToRemove)
{
	if (!CanRemoveWeapons())
		return false;

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

bool USf_Equipment::CycleWeapons(const ENavigationDirectionType Direction)
{
	if (!bAllowCycling)
		return false;
	
	if (OwnedWeapons.Num() <= 0)
		return false;
	
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

	return EquipWeaponBySlot(NextSlot);
}

bool USf_Equipment::EquipWeaponByReference(AWeaponBase* Weapon)
{
	if (CanEquipWeapon(Weapon))
		return ActivateWeapon(Weapon);
	return false;
}

bool USf_Equipment::EquipWeaponBySlot(int Slot)
{
	if (!CanEquipWeapons())
		return false;
	
	AWeaponBase* Weapon = nullptr;
	if (GetWeaponBySlot(Slot, Weapon))
		return ActivateWeapon(Weapon);

	UE_LOG(EquipmentComponent, Error, TEXT("The weapon slot you're trying to activate is invalid"));
	return false;
}

bool USf_Equipment::CanEquipWeapon(const AWeaponBase* WeaponToEquip) const
{
	if (!CanEquipWeapons())
		return false;

	if (!IsValid(WeaponToEquip))
		return false;
	
	return  true;
}

bool USf_Equipment::CanEquipWeapons() const
{
	return IS_ACTION_ALLOWED(EquippedWeapon,EquipWeapon);
}

bool USf_Equipment::CanUnequipWeapon() const
{
	return IS_ACTION_ALLOWED(EquippedWeapon,UnequipWeapon);
}

bool USf_Equipment::UnequipWeapon(bool HideWeapon)
{
	if (!CanUnequipWeapon())
		return false;
	
	if (!IsValid(EquippedWeapon))
		return false;

	AWeaponBase* OldWeapon = EquippedWeapon;
	EquippedWeapon->OnUnequip();
	if (HideWeapon)
		SetWeaponActive(EquippedWeapon, nullptr, false);
	
	EquippedWeapon = nullptr;
	OnWeaponChange.Broadcast(nullptr, OldWeapon);

	return true;
}

bool USf_Equipment::CanActivateWeapons() const
{
	return IS_ACTION_ALLOWED(EquippedWeapon,ActivateWeapon);
}

bool USf_Equipment::Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, EFireBlock& OutFireBlock)
{
	if (!IsEquipped())
	{
		OutFireBlock = EFireBlock::NoWeapon;
		return false;
	}
	
	return EquippedWeapon->Fire(InputSignal,FireType,OutHitResult,OutFireBlock);
}

bool USf_Equipment::Reload() const
{
	if (!IsEquipped())
		return false;

	float MontageTime = 0;
	return EquippedWeapon->Reload(MontageTime);
}


bool USf_Equipment::Reload(float &OutMontageTime)
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Reload(OutMontageTime);
}

bool USf_Equipment::InstantReload()
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->InstantReload();
}

void USf_Equipment::StopReloading()
{
	if (!IsEquipped())
		return;
	
	EquippedWeapon->StopReloading();
}

bool USf_Equipment::IsReloading() const
{
	if (!IsEquipped())
		return false;

	return  EquippedWeapon->IsReloading();
}

bool USf_Equipment::IsOnMeleeCooldown() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->IsOnMeleeCooldown();
}

bool USf_Equipment::Aim()
{
	if (!IsEquipped())
		return false;

	EquippedWeapon->AimDownSight();
	return true;
}

void USf_Equipment::StopAiming()
{
	if (!IsEquipped())
		return;
	
	EquippedWeapon->StopAiming();
}

bool USf_Equipment::IsOnFireCooldown() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsOnFireCooldown();
}

bool USf_Equipment::Melee()
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Melee();
}


// Called every frame
void USf_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int NewFlags = GetCompressedFlags();
	if (NewFlags != CurrentEquipmentFlags)
		OnEquipmentFlagsChange.Broadcast(CurrentEquipmentFlags, NewFlags);
	CurrentEquipmentFlags = NewFlags;
}


bool USf_Equipment::CanMelee() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->CanMelee();
}

FWeaponConfig USf_Equipment::GetWeaponConfig()
{
	if (!IsEquipped())
	{
		UE_LOG(EquipmentComponent, Error, TEXT("Cannot get config because no weapon is equipped"))
		return FWeaponConfig();
	}

	return  GetActiveWeapon()->GetWeaponConfig();
}


int USf_Equipment::GetCompressedFlags() const
{
	int EquipmentFlags = 0;

	USf_FunctionLibrary::SetBit(IsEquipped(),EquipmentFlags,EEquipmentFlags::EquipmentState_Equipped);
	USf_FunctionLibrary::SetBit(IsOnFireCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_FireCooldown);
	USf_FunctionLibrary::SetBit(IsAiming(),EquipmentFlags,EEquipmentFlags::EquipmentState_Aiming);
	USf_FunctionLibrary::SetBit(IsReloading(),EquipmentFlags,EEquipmentFlags::EquipmentState_Reloading);
	USf_FunctionLibrary::SetBit(IsOnMeleeCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_MeleeCooldown);

	return EquipmentFlags;
}

bool USf_Equipment::CheckFlag(EEquipmentFlags EquipmentFlag) const
{
	return USf_FunctionLibrary::CheckBitFlag(GetCompressedFlags(),EquipmentFlag);
}

bool USf_Equipment::CheckFlagForState(EEquipmentFlags EquipmentFlag, int StateToCheck) const
{
	return USf_FunctionLibrary::CheckBitFlag(StateToCheck,EquipmentFlag);
}

bool USf_Equipment::GetSlotByWeapon(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = INDEX_NONE;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex = OwnedWeapons.Find(WeaponBase);
	return HasWeapon(WeaponBase);
}

bool USf_Equipment::GetWeaponBySlot(int Index, AWeaponBase*& OutWeaponBase) const
{
	OutWeaponBase = nullptr;
	if (Index < 0 || Index >= OwnedWeapons.Num())
		return false;

	OutWeaponBase = OwnedWeapons[Index];
	return true;
}

void USf_Equipment::SetWeaponActive(AWeaponBase* Weapon,AWeaponBase* OtherWeapon, const bool Active)
{
	if (!HasWeapon(Weapon))
		return;

	Weapon->SetWeaponActive(Active, OtherWeapon);
}

bool USf_Equipment::ActivateWeapon(AWeaponBase* Weapon)
{
	if (!CanActivateWeapons())
	{
		return false;
	}
	
	if (!IsValid(Weapon))
	{
		return false;
	}
	
	if (!HasWeapon(Weapon))
	{
		return false;
	}
		
	AWeaponBase* OldWeapon = EquippedWeapon;
	if (OldWeapon)
	{
		OldWeapon->OnUnequip();
		SetWeaponActive(OldWeapon, Weapon, false);
	}
	
	EquippedWeapon = Weapon;
	EquippedWeapon->OnEquip();
	SetWeaponActive(EquippedWeapon, OldWeapon,true);

	OnWeaponChange.Broadcast(EquippedWeapon, OldWeapon);
	return true;
}

void USf_Equipment::AttachToParentMesh()
{
	USceneComponent* Parent =  GetAttachParent();
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	AttachToComponent(Parent,AttachRules,WeaponAttachmentSocket);
}

TArray<FName> USf_Equipment::GetWeaponAttachmentSocketOptions() const
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

