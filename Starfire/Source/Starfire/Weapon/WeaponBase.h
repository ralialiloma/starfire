// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireBlocks.h"
#include "FireType.h"
#include "WeaponConfig.h"
#include "GameFramework/Actor.h"
#include "Starfire/Utility/InputSignalType.h"
#include "Starfire/Animation/WeaponAnimData.h"
#include "WeaponBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Weapon, Display, Display);

UCLASS(BlueprintType)
class STARFIRE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void Tick(float DeltaTime) override;

#pragma region Properties

protected:
	
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;
	
	//Config
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FWeaponConfig WeaponConfig;

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
	FTimerHandle ReloadTimer = FTimerHandle();

	UPROPERTY()
	AActor* WeaponOwner = nullptr;

#pragma endregion

#pragma region Functions
public:

	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	int GetAmmoCount() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	FTransform GetMuzzleTransform() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	FWeaponConfig GetWeaponConfig() const;
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	AActor* GetWeaponOwner() const;
	
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	void SetWeaponActive(bool Active);
	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	void OnEquip(AActor* NewHolder);

protected:

	//Animation
	float PlayMontage(UAnimMontage* MontageToPlay);
	float PlayMontage (EWeaponAnimationMontageType MontageType);
	void StopMontage(UAnimMontage* MontageToStop);
	
#pragma endregion

#pragma region Fire
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponBase")
	bool CanFire(EInputSignalType InputSignal, EFireType FireType, TEnumAsByte<EFireBlock>& OutBlock);
	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsOnFireCooldown();
	
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Fire(const EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult,	TEnumAsByte<EFireBlock>& OutFireBlock);

protected:
	
	void GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd);
	bool CheckInputSignalType(EInputSignalType InputSignalType);
	
	void DoFire(FHitResult& OutHitResult);
	void FireTraces(FHitResult& OutHitResult);
	
#pragma endregion

#pragma region Reload
public:

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool IsReloading();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Reload();
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopReloading();
	
#pragma endregion

#pragma region Aim
public:

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool IsAiming();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void AimDownSight();
	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	void StopAiming();
	
protected:
	
	void AimDownSight(float Alpha);
	
#pragma endregion
	
#pragma region Melee
public:
	
	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsOnMeleeCooldown();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponBase")
	bool CanMelee();

	UFUNCTION(BlueprintCallable, Category="WeaponBase")
	bool Melee();
	
protected:
	
	void MeleeTraces();
	void ApplyMelee(AActor* ActorToApplyOn, FVector Start, FVector End, FVector Direction);
	void DoMelee();

#pragma endregion
	
};
