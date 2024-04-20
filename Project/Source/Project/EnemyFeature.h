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

protected:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	AEnemyAI* OwningAIHolder = nullptr;

public:
	void Initialize(AEnemyAI* Holder);
	
};
