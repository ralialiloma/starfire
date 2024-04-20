// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BetterObject.h"
#include "EnemyFeature.generated.h"

class AEnemyAI;

UCLASS(Blueprintable)
class PROJECT_API UEnemyFeature : public UBetterObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool RunInTick = false;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	float UpdateRateInSeconds = 1;

protected:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	AEnemyAI* OwningAIHolder = nullptr;


public:
	void Initialize(AEnemyAI* Holder);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AIDeviations")
	FVector CalculatePositionDeviation(FVector Position, float Accuracy);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AIDeviations")
	FRotator CalculateRotationDeviation(FRotator Rotation, float Accuracy);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIDeviations")
	float CalculateDeviationValue(float ValuetoDeviateFrom, float Accuracy);
	
};
