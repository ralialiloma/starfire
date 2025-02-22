﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponFeature.h"
#include "GameFramework/Actor.h"
#include "Starfire/Animation/WeaponMontageEventPackage.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "StructsAndEnums/FireBlocks.h"
#include "StructsAndEnums/FireType.h"
#include "StructsAndEnums/WeaponConfig.h"
#include "WeaponBase.generated.h"

class UWeaponFeatureConfig;
class USf_Equipment;
DEFINE_LOG_CATEGORY_STATIC(SF_Weapon, Display, Display);

#define IS_ACTION_ALLOWED(WeaponBase, ActionType) \
((WeaponBase) ? (WeaponBase)->IsActionAllowed(Sf_GameplayTags::Gameplay::Weapon::Action::ActionType) : true)


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnWeaponHit,
	const float, Damage,
	FGameplayTag,DamageType,
	FHitResult,HitResult,
	USf_DamageController*, DamageController);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponKill,
	FHitResult,HitResult,
	USf_DamageController*, KilledDamageController);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAim,const bool, IsAiming);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailFire,const EFireBlock, FireBlock);



UCLASS(BlueprintType)
class STARFIRE_API AWeaponBase : public AActor, public IPrimaryInteract
{
	GENERATED_BODY()

public:
	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	
	bool TryAddFeatureByConfigClass(const TSubclassOf<UWeaponFeatureConfig> FeatureConfigType);
	bool TryAddFeature(UWeaponFeatureConfig* ComponentConfig);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon", meta = (DeterminesOutputType = "Class"))
	UWeaponFeature* GetFeatureByClass(const TSubclassOf<UWeaponFeature> Class);
	template <typename FeatureType>
	FeatureType* GetFeatureByClass();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

private:
	static TSet<TSubclassOf<UWeaponFeatureConfig>> GetAllStartConfigs();
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
	USkeletalMeshComponent* GetSkeletalMesh() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	APawn* GetWeaponOwner() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	USf_Equipment* GetOwningSfEquipmentComp() const;
	
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void SetWeaponActive(const bool Active, AWeaponBase* OtherWeapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void SetNewHolder(USf_Equipment* NewHolder);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnPickup(USf_Equipment* NewHolder);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnDrop();
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	void SetCollisionAndPhysics(bool bActive);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual  void OnEquip(float& OutMontageTime);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void OnUnequip(float& OutMontageTime);

	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual bool IsActionAllowed(UPARAM(meta=(Categories="Gameplay.Weapon.Action"))FGameplayTag ActionType) const;
protected:
	//Animation
	float ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType WeaponAnimationEventType, bool bIsStarting = true) const;
	void ExecuteAnimation(EWeaponAnimationEventType WeaponAnimationEventType, bool bIsStarting = true) const;

#pragma endregion

#pragma region Properties
public:
	//DestroyOnCollision
	UPROPERTY()
	bool bDestroyOnCollision = false;
	
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

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UWeaponFeatureConfig*> FeatueConfigs = TArray<UWeaponFeatureConfig* >();

	UPROPERTY()
	TArray<UWeaponFeature*> Features = TArray<UWeaponFeature* >();

private:
	//States
	UPROPERTY()
	bool bIsAiming =false;
	UPROPERTY()
	bool bActiveFireCooldown = false;
	UPROPERTY()
	bool bActiveMeleeCountdown = false;
	UPROPERTY()
	bool bIsMeleeing = false;
	
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

	//Melee
	UPROPERTY()
	TArray<AActor*> OverlappedMeleeActors = {};
	
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

public:
	UPROPERTY()
	FGameplayTag Tag;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponHit OnWeaponApplyDamage;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponKill OnWeaponKill;

	UPROPERTY(BlueprintAssignable)
	FOnFailFire OnFailFire;
protected:
	UFUNCTION(BlueprintNativeEvent,Category="WeaponBase")
	USceneComponent* GetMuzzleSceneComponent() const;
	
	void GetTracePoints(const FTransform& InFireTransform, FVector& OutStart, FVector& OutEnd) const;
	bool CheckInputSignalType(EInputSignalType InputSignalType);
	
	virtual void DoFire(FHitResult& OutHitResult);
	void FireTraces(FHitResult& OutHitResult);
	void TraceALongFireTransform(FHitResult& OutHitResult);
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
	void SetClip(int NewClip);

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	virtual bool CanReload();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopReloading();
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnReloadStart_BP;
	
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

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponAim OnWeaponAim;
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

	UFUNCTION()
	void HandleMeleeBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleMeleeEndOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex
	);
	
	void DoMelee();
	void ApplyMeleeToActor(AActor* ActorToApplyMeleeTo);

#pragma endregion

#pragma region Animation
public:
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void PlayWeaponAnimation(EWeaponAnimationEventType EventType) const;
#pragma endregion
	
};

template <typename FeatureType>
FeatureType* AWeaponBase::GetFeatureByClass()
{
	static_assert(TIsDerivedFrom<FeatureType, UWeaponFeature>::IsDerived, "FeatureType must be derived from UWepaonFeature");
	UWeaponFeature* FoundFeature =  GetFeatureByClass(FeatureType::StaticClass());
	if (!IsValid(FoundFeature))
		return nullptr;
	return Cast<FeatureType>(FoundFeature);
}
