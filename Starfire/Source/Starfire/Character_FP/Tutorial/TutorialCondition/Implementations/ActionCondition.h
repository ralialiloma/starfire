// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/Character_FP/Tutorial/TutorialCondition/TutorialConditions.h"
#include "ActionCondition.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UActionCondition : public UTutorialConditions
{
	GENERATED_BODY()

public:
	virtual bool EvaluateCondition_Implementation(ATutorialManager* TutorialManager) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTagContainer AllowedActions {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bExact = true;
};
