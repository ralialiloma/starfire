// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Starfire/Animation/WeaponMontageEventPackage.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "StructsAndEnums/FireBlocks.h"
#include "StructsAndEnums/FireType.h"
#include "StructsAndEnums/WeaponConfig.h"
#include "WeaponBase.generated.h"


class USf_Equipment;
DEFINE_LOG_CATEGORY_STATIC(SF_Weapon, Display, Display);

#define IS_ACTION_ALLOWED(WeaponBase, ActionType) \
((WeaponBase) ? (WeaponBase)->IsActionAllowed(Sf_GameplayTags::Gameplay::Weapon::Action::ActionType) : true)


UCLASS(BlueprintType)
class STARFIRE_API AWeaponBase : public AActor, public IPrimaryInteract
{
	GENERATED_BODY()

public:
	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn) override;

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	int GetAmmoCount() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	FTransform GetMuzzleTransform() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	FWeaponConfig GetWeaponConfig() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	APawn* GetWeaponOwner() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	USf_Equipment* GetOwningSfEquipmentComp() const;
	
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void SetWeaponActive(const bool Active, AWeaponBase* OtherWeapon);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnPickup(USf_Equipment* NewHolder);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnDrop();
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual  void OnEquip();
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnUnequip();

	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual bool IsActionAllowed(UPARAM(meta=(Categories="Gameplay.Weapon.Action"))FGameplayTag ActionType) const;

protected:
	//Animation
	float ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType WeaponAnimationEventType, bool bIsStarting = true) const;
	void ExecuteAnimation(EWeaponAnimationEventType WeaponAnimationEventType, bool bIsStarting = true) const;
#pragma endregion


#pragma region Properties

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;
	
	//Config
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FWeaponConfig WeaponConfig = FWeaponConfig();

	UPROPERTY(meta=(Categories="Gameplay.Weapon.Action"))
	TSet<FGameplayTag> ForbiddenActions;

private:
	//States
	UPROPERTY()
	bool bIsAiming =false;
	UPROPERTY()
	bool bActiveFireCooldown = false;
	UPROPERTY()
	bool bActiveMeleeCountdown = false;
	
	//Ammo
	UPROPERTY()
	int CurrentClip = 0;

	//TimerHandles
	UPROPERTY()
	FTimerHandle FireCooldown = FTimerHandle();
	UPROPERTY()
	FTimerHandle MeleeCooldown = FTimerHandle();
	UPROPERTY()
	APawn* WeaponOwner = nullptr;

	UPROPERTY()
	USf_Equipment* OwningEquipmentComponent = nullptr;

#pragma endregion

#pragma region Fire
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponBase")
	virtual bool CanFire(EInputSignalType InputSignal, EFireType FireType, EFireBlock& OutBlock);
	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsOnFireCooldown();
	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool HasEnoughAmmoToFire() const;
	
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Fire(const EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, EFireBlock& OutFireBlock);

protected:
	void GetTracePoints(const FTransform& InFireTransform, FVector& OutStart, FVector& OutEnd) const;
	bool CheckInputSignalType(EInputSignalType InputSignalType);
	
	virtual void DoFire(FHitResult& OutHitResult);
	void FireTraces(FHitResult& OutHitResult);
	void TraceALongFireTransform(FHitResult OutHitResult);
	virtual void ApplyDamage (const FHitResult& InHitResult) const;
	void ApplyRecoil(float Modifier = 1) const;
#pragma endregion

#pragma region Reload
#pragma region Functions

public:
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool IsReloading() const;

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Reload(float& OutMontageTime);

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool InstantReload();

	bool Reload();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	virtual bool CanReload();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopReloading();
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnReloadStopped_BP;
	FSf_VoidDelegate_CPP OnReloadStopped_CPP;

	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnReloadFinish_BP;
	FSf_VoidDelegate_CPP OnReloadFinish_CPP;
private:
	UPROPERTY()
	FTimerHandle ReloadTimer = FTimerHandle();

#pragma endregion


#pragma endregion 

#pragma region Aim
public:

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool IsAiming();


	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	virtual  bool CanAim();
		
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void AimDownSight();
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopAiming();
	
#pragma endregion
	
#pragma region Melee
public:
	
	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsOnMeleeCooldown();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponBase")
	virtual bool CanMelee();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Melee();
	
protected:
	
	void MeleeTraces();
	void ApplyMelee(AActor* ActorToApplyOn, FVector Start, FVector End, FVector Direction);
	void DoMelee();

#pragma endregion
	
};
