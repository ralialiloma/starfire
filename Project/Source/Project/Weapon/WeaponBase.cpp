// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot =  CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SkeletalMesh =  CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::FireTraces()
{
}

