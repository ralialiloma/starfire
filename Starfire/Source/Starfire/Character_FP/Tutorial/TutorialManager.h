// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UCLASS()
class STARFIRE_API ATutorialManager : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TutorialManager", meta = (Categories = "Gameplay.ActionLogger", ExposeOnSpawn))
	TArray<FGameplayTag> TutorialActions {};
};
