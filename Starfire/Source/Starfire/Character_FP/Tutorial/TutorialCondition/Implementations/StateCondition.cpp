// Fill out your copyright notice in the Description page of Project Settings.

#include "StateCondition.h"

#include "Starfire/Character_FP/Tutorial/TutorialManager.h"

bool UStateCondition::EvaluateCondition_Implementation(ATutorialManager* TutorialManager)
{
	if (bExact)
		return AllowedStates.HasTagExact(TutorialManager->GetCurrentStateTag());
	return AllowedStates.HasTag(TutorialManager->GetCurrentStateTag());
}
