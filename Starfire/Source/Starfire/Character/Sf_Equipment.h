// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Starfire/Weapon/FireBlocks.h"
#include "Starfire/Weapon/FireType.h"
#include "Starfire/Weapon/WeaponBase.h"
#include "Starfire/Utility/InputSignalType.h"
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
#pragma endregion

UCLASS(ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class STARFIRE_API USF_Equipment : public USceneComponent
{
	GENERATED_BODY()

public:
	
	USF_Equipment();
	
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#pragma region Functions
public:

	//Gets
	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetActiveWeapon() const;
	UFUNCTION(BlueprintCallable)
	FWeaponAnimData GetEquippedAnimationData() const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsEquipped() const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsAiming() const;
	UFUNCTION(BlueprintCallable)
	bool IsReloading() const;
	UFUNCTION(BlueprintCallable)
	bool IsMeleeOnCooldown() const;
	UFUNCTION(BlueprintCallable)
	bool IsFireOnCooldown() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanMelee() const;

	//New Weapons
	UFUNCTION(BlueprintCallable)
	void AddWeapon(AWeaponBase* WeaponToAdd, bool Equip, int& Index);
	UFUNCTION(BlueprintCallable)
	void AddWeaponByClass(TSubclassOf<AWeaponBase> ActorClass, bool Equip, int& Index);

	//Actions
	UFUNCTION(BlueprintCallable)
	bool Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock);
	UFUNCTION(BlueprintCallable)
	bool Reload();
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
	bool GetSlotByWeapon(AWeaponBase* WeaponBase, int& OutIndex) const;
	UFUNCTION(BlueprintCallable)
	bool GetWeaponBySlot(int Index, AWeaponBase*& OutWeaponBase) const;
	
#pragma endregion

#pragma region Properties
public:

	UPROPERTY(BlueprintAssignable, Category = "Equipment|State")
	FOnEquipmentStateChange OnEquipmentFlagsChange;

protected:

	//References
	UPROPERTY()
	TArray<AWeaponBase*> OwnedWeapons;
	UPROPERTY()
	AWeaponBase* EquippedWeapon;
	
	UPROPERTY()
	int CurrentEquipmentFlags;

#pragma endregion

#pragma region Attachment
protected:
	//Attachment
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly , Category = "Sockets",meta=(GetOptions="GetWeaponAttachmentSocketOptions"))
	FName WeaponAttachmentSocket;

private:
	UFUNCTION()
	TArray<FName> GetWeaponAttachmentSocketOptions() const;
	
	void AttachToParentMesh();
#pragma endregion

};
