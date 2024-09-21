// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	//Events



	//Actor
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Interface
public:
	void Fire (
	FTransform InFireTransform,
	AActor* InActorFiring,
	FHitResult& OutHitResult,
	UAnimMontage* &OutMontageToPlay,
	float&OutRecoil);

	void Reload(UAnimMontage*& OutMontageToPlay);

	void OnEquip(UAnimMontage*& OutMontageToPlay, AActor* NewOwner);
	
	

	//Internal
private:

	//Fire
	void FireTraces(FTransform FireTransform, AActor* FiringActor, FHitResult& OutHitResult);
	void GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd);

	//Cooldown
	bool IsInCooldown();
	void ResetFireCooldown();

};
