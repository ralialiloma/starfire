// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/Character_FP/Tutorial/TutorialCondition/TutorialConditions.h"
#include "AreaCondition.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UAreaCondition : public UTutorialConditions
{
	GENERATED_BODY()

public:
	virtual bool EvaluateCondition_Implementation(ATutorialManager* TutorialManager) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameplayContainerMatchType MatchType = EGameplayContainerMatchType::Any;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Region"))
	FGameplayTagContainer AllowedRegions {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bInvert = false;
};
