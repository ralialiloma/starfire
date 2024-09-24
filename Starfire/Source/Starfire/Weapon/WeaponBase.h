// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireBlocks.h"
#include "FireType.h"
#include "WeaponConfig.h"
#include "GameFramework/Actor.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Animation/FWeaponAnimData.h"
#include "WeaponBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Weapon, Display, Display);

UCLASS(BlueprintType)
class STARFIRE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	//Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	//Config
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Config")
	FWeaponConfig InitialConfig;

	//Transient
private:
	UPROPERTY()
	FWeaponConfig ActiveConfig;
	
	UPROPERTY()
	bool bIsAiming =false;

	UPROPERTY()
	bool bActiveFireCooldown = false;
	
	UPROPERTY()
	bool bActiveMeleeCountdown = false;
	

	UPROPERTY()
	int CurrentClip = 0;

	UPROPERTY()
	FTimerHandle FireCooldown = FTimerHandle();

	UPROPERTY()
	FTimerHandle MeleeCooldown = FTimerHandle();

	UPROPERTY()
	FTimerHandle ReloadTimer = FTimerHandle();

	UPROPERTY()
	AActor* WeaponHolder = nullptr;

	//Events

	//Actor
protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void Tick(float DeltaTime) override;

	//Interface
public:

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool Fire(
		const EInputSignalType InputSignal,
		EFireType FireType,
		FHitResult& OutHitResult,
		TEnumAsByte<EFireBlock>& OutFireBlock);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool Reload();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsReloading();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	void StopReloading();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool Melee();
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="WeaponBase")
	bool CanMelee();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	int GetAmmoCount();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	FWeaponConfig GetActiveConfig();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	void SetWeapon(bool Active);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	void OnEquip(AActor* NewHolder);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category="WeaponBase")
	bool CanFire(EInputSignalType InputSignal, EFireType FireType, TEnumAsByte<EFireBlock>& OutBlock);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsInFireCooldown();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsInMeleeCooldown();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool IsAiming();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void AimDownSight();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopAiming();


//Internal
	//Fire
private:
	void GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd);
	bool CheckInputSignalType(EInputSignalType InputSignalType);
protected:
	void DoFire (
		FHitResult& OutHitResult);
	void FireTraces(FHitResult& OutHitResult);

	//Animation
private:
	float PlayMontage(UAnimMontage* MontageToPlay);
	float PlayMontage (EWeaponAnimationMontageType MontageType);
	void AimDownSight(float Alpha);
	void StopMontage(UAnimMontage* MontageToStop);

	//Melee
private:
	void DoMelee();
protected:
	void MeleeTraces();
	void ApplyMelee(AActor* ActorToApplyOn, FVector Start, FVector End, FVector Direction);
	
};
