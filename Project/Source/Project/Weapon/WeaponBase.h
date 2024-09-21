// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponConfig.h"
#include "GameFramework/Actor.h"
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

	//Events

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//Fire
private:
	void FireTraces(FTransform FireTransform, AActor* FiringActor);
	void GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd);
};
