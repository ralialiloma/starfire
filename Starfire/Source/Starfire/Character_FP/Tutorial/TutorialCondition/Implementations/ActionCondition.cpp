// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCondition.h"

#include "Starfire/Character_FP/Tutorial/TutorialManager.h"

bool UActionCondition::EvaluateCondition_Implementation(ATutorialManager* TutorialManager)
{
	if (bExact)
		return AllowedActions.HasAnyExact(TutorialManager->GetCurrentTutorialActions());
	
	return AllowedActions.HasAny(TutorialManager->GetCurrentTutorialActions());
}
