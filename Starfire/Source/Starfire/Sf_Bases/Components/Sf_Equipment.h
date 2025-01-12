// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/FireBlocks.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/FireType.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Utility/NavigationDirectionType.h"
#include "Sf_Equipment.generated.h"

#pragma region Enums and Structs
UENUM(BlueprintType,meta = (Bitflags))
enum EEquipmentFlags
{
	EquipmentState_NoFlags = 0 UMETA(Hidden),
	EquipmentState_Equipped = 1 << 0,  
	EquipmentState_Aiming = 1 << 1,
	EquipmentState_FireCooldown = 1 << 2,  
	EquipmentState_Reloading = 1 << 3,   
	EquipmentState_MeleeCooldown = 1 << 4    
};
ENUM_CLASS_FLAGS(EEquipmentFlags)

#pragma endregion

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentStateChange, int, PreviousState, int, UpdatedState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChange, AWeaponBase*, NewWeapon, AWeaponBase*, OldWeapon);
#pragma endregion

UCLASS(ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class STARFIRE_API USf_Equipment : public USceneComponent
{
	GENERATED_BODY()

public:
	
	USf_Equipment();
	
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Functions
public:
	//Gets
	UFUNCTION(BlueprintCallable)
	bool HasWeapon(AWeaponBase* WeaponBase) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponBase* GetActiveWeapon() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponBase* GetWeaponByClass(TSubclassOf<AWeaponBase> WeaponClass);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetActiveSlot() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponAnimData_FP GetEquippedAnimationData_FP() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponAnimData_TP GetEquippedAnimationData_TP() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEquipped() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAiming() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsReloading() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOnMeleeCooldown() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOnFireCooldown() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanMelee() const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FWeaponConfig GetWeaponConfig();

	//Weapon Management
	UFUNCTION(BlueprintCallable)
	void AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int& Slot, float& OutMontageTIme);
	UFUNCTION(BlueprintCallable)
	void AddWeaponByClass(const TSubclassOf<AWeaponBase> WeaponClassToAdd, const bool Equip, int& Index, float& OutMontageTIme);

	UFUNCTION(BlueprintCallable)
	bool CanAddWeapon(AWeaponBase* WeaponToAdd) const;

	UFUNCTION(BlueprintCallable)
	bool CanAddWeapons() const;

	UFUNCTION(BlueprintCallable)
	bool CanRemoveWeapon(AWeaponBase* WeaponToRemove) const;
	UFUNCTION(BlueprintCallable)
	bool CanRemoveWeapons() const;
	UFUNCTION(BlueprintCallable)
	bool RemoveWeapon(AWeaponBase* WeaponToRemove, float& OutMontageTime);
	UFUNCTION(BlueprintCallable)
	bool RemoveWeaponActiveWeapon(float& OutMontageTime);
	UFUNCTION(BlueprintCallable)
	bool RemoveWeaponByClass(TSubclassOf<AWeaponBase> WeaponClassToRemove, float& OutMontageTime);
	UFUNCTION(BlueprintCallable)
	bool CycleWeapons(const ENavigationDirectionType Direction, float& OutMontageTime);

	UFUNCTION(BlueprintCallable)
	bool EquipWeaponByReference(AWeaponBase* Weapon, float& OutMontageTime);
	UFUNCTION(BlueprintCallable)
	bool EquipWeaponBySlot(int Slot, float& OutMontageTime);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanEquipWeapon(const AWeaponBase* WeaponToEquip)const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanEquipWeapons()const;
	UFUNCTION(BlueprintCallable)
	bool CanUnequipWeapon() const;

	UFUNCTION(BlueprintCallable)
	bool UnequipWeapon(bool HideWeapon, float& OutMontageTime);
	
	bool UnequipWeaponInstant(bool HideWeapon);

	UFUNCTION(BlueprintCallable)
	bool CanActivateWeapons() const;
	UFUNCTION(BlueprintCallable)
	bool ActivateWeapon(AWeaponBase* Weapon, float& OutTotalMontageTime);

	//Actions
	UFUNCTION(BlueprintCallable)
	bool Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, EFireBlock& OutFireBlock);
	UFUNCTION(BlueprintCallable)
	bool Reload(float &OutMontageTime);
	UFUNCTION(BlueprintCallable)
	bool InstantReload();
	bool Reload() const;
	UFUNCTION(BlueprintCallable)
	void StopReloading();
	UFUNCTION(BlueprintCallable)
	bool Aim();
	UFUNCTION(BlueprintCallable)
	void StopAiming();
	UFUNCTION(BlueprintCallable)
	bool Melee();

	//Flags
	UFUNCTION(BlueprintCallable)
	int GetCompressedFlags() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckFlag(EEquipmentFlags EquipmentFlag) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckFlagForState(EEquipmentFlags EquipmentFlag, int StateToCheck) const;

protected:

	UFUNCTION(BlueprintCallable)
	void SetWeaponActive(AWeaponBase* Weapon, AWeaponBase* OtherWeapon, const bool Active);
	
	UFUNCTION(BlueprintCallable)
	bool GetSlotByWeapon(AWeaponBase* WeaponBase, int& OutIndex) const;
	UFUNCTION(BlueprintCallable)
	bool GetWeaponBySlot(int Index, AWeaponBase*& OutWeaponBase) const;
	

	
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FOnEquipmentStateChange OnEquipmentFlagsChange;

	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FOnWeaponChange OnWeaponChange;

protected:
	UPROPERTY(BlueprintReadOnly,EditInstanceOnly)
	bool bAllowCycling = false;

	UPROPERTY()
	TArray<AWeaponBase*> OwnedWeapons;
	UPROPERTY()
	AWeaponBase* EquippedWeapon;
	UPROPERTY()
	int CurrentEquipmentFlags;

	
	FTimerHandle UnequipHandle;


#pragma endregion

#pragma region Attachment
protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly , Category = "Sockets",meta=(GetOptions="GetWeaponAttachmentSocketOptions"))
	FName WeaponAttachmentSocket;
private:
	UFUNCTION()
	TArray<FName> GetWeaponAttachmentSocketOptions() const;
	
	void AttachToParentMesh();
#pragma endregion

};
