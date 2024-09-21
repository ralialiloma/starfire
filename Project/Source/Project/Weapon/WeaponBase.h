// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireBlocks.h"
#include "FireType.h"
#include "WeaponConfig.h"
#include "GameFramework/Actor.h"
#include "Project/Animation/FWeaponAnimData.h"
#include "WeaponBase.generated.h"

UCLASS()
class PROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	//Components

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

	//Config
	UPROPERTY(BlueprintReadWrite)
	FWeaponConfig Config;

	//Transient
private:
	UPROPERTY()
	bool bIsAiming;

	UPROPERTY()
	bool bActiveFireCooldown;

	UPROPERTY()
	int CurrentClip;

	UPROPERTY()
	FTimerHandle FireCooldown;

	UPROPERTY()
	FWeaponAnimData CurrentWeaponAnimData;

	UPROPERTY()
	AActor* WeaponHolder;

	//Events

	//Actor
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Interface
public:

	bool Fire(
		const EInputSignalType InputSignal,
		EFireType FireType,
		FHitResult& OutHitResult,
		TEnumAsByte<EFireBlock>& OutFireBlock);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	float Reload();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	float Melee();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	int GetAmmoCount();

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	void SetWeapon(bool Active);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	void OnEquip(AActor* NewHolder);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool CanFire(EInputSignalType InputSignal, EFireType FireType, TEnumAsByte<EFireBlock>& OutBlock);

	UFUNCTION(BlueprintCallable,Category="WeaponBase")
	bool IsInCooldown();


//Internal
	//Fire
private:
	void GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd);
	bool CheckInputSignalType(EInputSignalType InputSignalType);
	void ResetFireCooldown();
protected:
	void DoFire (
		FHitResult& OutHitResult);
	void FireTraces(FHitResult& OutHitResult);

	//Animation
private:
	float PlayMontage(UAnimMontage* MontageToPlay);
	float PlayMontage (EWeaponAnimationMontageType MontageType);

	//Melee
private:
	void DoMelee();




};
