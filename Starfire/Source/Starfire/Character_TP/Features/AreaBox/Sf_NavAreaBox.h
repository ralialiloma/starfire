// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Sf_NavAreaBox.generated.h"

UCLASS()
class STARFIRE_API ASf_NavAreaBox : public AActor
{
	GENERATED_BODY()

public:
	ASf_NavAreaBox();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

public:
	virtual void Tick(float DeltaTime) override;
};