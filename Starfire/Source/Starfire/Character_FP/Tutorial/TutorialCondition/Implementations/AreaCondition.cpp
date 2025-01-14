// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaCondition.h"

#include "Starfire/Character_FP/Tutorial/TutorialManager.h"

bool UAreaCondition::EvaluateCondition_Implementation(ATutorialManager* TutorialManager)
{
	for (auto Region : AllowedRegions)
	{
		bool bEntered = TutorialManager->IsInRegion(Region);
		if (MatchType == EGameplayContainerMatchType::Any && bEntered)
			return !bInvert;

		if (MatchType == EGameplayContainerMatchType::All && !bEntered)
			return bInvert;
	}

	if (MatchType == EGameplayContainerMatchType::Any)
		return bInvert;

	if (MatchType == EGameplayContainerMatchType::All)
		return !bInvert;

	return false;
}
