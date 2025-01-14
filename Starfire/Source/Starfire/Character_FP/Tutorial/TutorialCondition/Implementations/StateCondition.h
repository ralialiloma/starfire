// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/Character_FP/Tutorial/TutorialCondition/TutorialConditions.h"
#include "StateCondition.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UStateCondition : public UTutorialConditions
{
	GENERATED_BODY()

public:
	virtual bool EvaluateCondition_Implementation(ATutorialManager* TutorialManager) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Tutorial.State"))
	FGameplayTagContainer AllowedStates {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bExact = true;
};
