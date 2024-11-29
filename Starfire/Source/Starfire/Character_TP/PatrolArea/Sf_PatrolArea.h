// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Sf_PatrolArea.generated.h"

UCLASS()
class STARFIRE_API ASf_PatrolArea : public AActor
{
	GENERATED_BODY()

public:
	ASf_PatrolArea();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInBox(const FVector& LocationToTest)  const;

protected:


	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

public:
	virtual void Tick(float DeltaTime) override;
};