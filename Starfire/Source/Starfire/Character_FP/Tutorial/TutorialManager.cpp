// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialManager.h"

#include "DebugFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/ActionLogger/ActionLogger.h"
#include "Starfire/Shared/BreakerTarget/Sf_BreakerPillar.h"
#include "TutorialCondition/TutorialConditions.h"
#include "TutorialCondition/TutorialConditonSettings.h"

void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();

	for (auto Actor : EnemySpawns)
	{
		if (!Actor)
			return;
		
		EnemySpawnTransforms.Add(Actor->GetActorTransform());
		EnemyClass = Actor->GetClass();
		Actor->Destroy();
	}
	
	for (auto Actor : CrystalSpawns)
	{
		if (!Actor)
			return;
		
		CrystalSpawnTransforms.Add(Actor->GetActorTransform());
		CrystalClass = Actor->GetClass();
		Actor->Destroy();
	}
}

void ATutorialManager::ToggleTutorial(FTransform InReturnTransform)
{
	if (IsTutorialActive())
		EndTutorial();
	else
		StartTutorial(InReturnTransform);
}

void ATutorialManager::StartTutorial(FTransform InReturnTransform)
{
	ReturnTransform = InReturnTransform;
	TutorialActive = true;
	TutorialState = -1;

	if (!TutorialStart || !TutorialEnd)
	{
		UDebugFunctionLibrary::DebugError(this, "Tutorial Start or End not Declared!");
		return;
	}
	
	GetPlayerPawn()->TeleportTo(TutorialStart->GetActorLocation(), TutorialStart->GetActorRotation());
	TutorialEnd->FullRestore();
	TutorialEnd->OnBreak_BP.AddDynamic(this, &ATutorialManager::EndTutorial);

	//Start Tutorial
	UActionLoggerSubSystem* ActionLogger = UActionLoggerSubSystem::Get(GetWorld());
	ActionLogger->OnActionReport.AddDynamic(this, &ATutorialManager::OnActionLogged);
	CompletedTutorialActions = FGameplayTagContainer();
	CurrentRegion = FGameplayTag();
	for (auto Transform : EnemySpawnTransforms)
	{
		ActorsToClean.Add(GetWorld()->SpawnActor(EnemyClass, &Transform));
	}
	for (auto Transform : CrystalSpawnTransforms)
	{
		ActorsToClean.Add(GetWorld()->SpawnActor(CrystalClass, &Transform));
	}
	SelectNewState();

	OnTutorialStart.Broadcast();
}

void ATutorialManager::EndTutorial()
{
	if (!TutorialStart || !TutorialEnd)
	{
		UDebugFunctionLibrary::DebugError(this, "Tutorial Start or End not Declared!");
		return;
	}

	GetPlayerPawn()->TeleportTo(ReturnTransform.GetLocation(), ReturnTransform.Rotator());
	TutorialEnd->OnBreak_BP.RemoveDynamic(this, &ATutorialManager::EndTutorial);
	
	//End Tutorial
	UActionLoggerSubSystem* ActionLogger = UActionLoggerSubSystem::Get(GetWorld());
	ActionLogger->OnActionReport.RemoveDynamic(this, &ATutorialManager::OnActionLogged);

	for (int i = ActorsToClean.Num() - 1; i >= 0; --i)
	{
		AActor* ActorToClean = ActorsToClean[i];
		ActorsToClean.RemoveAt(i);
		ActorToClean->Destroy();
	}

	ReturnTransform = FTransform();
	TutorialActive = false;
	
	OnTutorialEnd.Broadcast();
}

void ATutorialManager::ForceProgressTutorial()
{
	for (auto Action : GetCurrentTutorialActions())
	{
		if (!IsTutorialActionComplete(Action))
			OnActionLogged(FCachedActionLogMessage(FActionLog(Action)), 0);
	}
}

bool ATutorialManager::IsTutorialActive() const
{
	return TutorialActive;
}

bool ATutorialManager::AreTutorialActionsComplete(FGameplayTagContainer ActionTag) const
{
	return CompletedTutorialActions.HasAllExact(ActionTag);
}

bool ATutorialManager::IsTutorialActionComplete(FGameplayTag ActionTag) const
{
	return CompletedTutorialActions.HasTagExact(ActionTag);
}

bool ATutorialManager::IsInRegion(FGameplayTag ActionTag) const
{
	return ActionTag.MatchesTag(CurrentRegion);
}

int32 ATutorialManager::GetNumTutorialActionLeft() const
{
	return TutorialStateActionsPairings.Num() - CompletedTutorialActions.Num();
}

int32 ATutorialManager::GetCurrentStateIndex() const
{
	return TutorialState;
}

FGameplayTag ATutorialManager::GetCurrentStateTag() const
{
	return GetStateTag(TutorialState);
}

FGameplayTagContainer ATutorialManager::GetCurrentTutorialActions() const
{
	return GetStateTutorialActions(TutorialState);
}

FGameplayTagContainer ATutorialManager::GetAllTutorialActionsRaw() const
{
	FGameplayTagContainer ActionTags {};
	for (auto Action : TutorialStateActionsPairings)
	{
		ActionTags.AppendTags(Action.TutorialActions);
	}
	return ActionTags;
}

bool ATutorialManager::EvaluateTutorialConditions(FTutorialConditionSettings Conditions)
{
	return Conditions.Evaluate(this);
}

void ATutorialManager::OnActionLogged(FCachedActionLogMessage NewMessage, int MessageCount)
{
	FGameplayTag CurrentLoggedAction = NewMessage.Message.RelatedTag;

	if (!GetAllTutorialActionsRaw().HasTagExact(CurrentLoggedAction))
		return;
	
	if (GetCurrentTutorialActions().HasTagExact(CurrentLoggedAction))
	{
		CompletedTutorialActions.AddTag(CurrentLoggedAction);

		//Check For all current State Actions
		FGameplayTagContainer CompletedActions {};
		for (auto Action : GetCurrentTutorialActions())
		{
			if (CompletedActions.HasTagExact(Action))
				CompletedActions.AddTag(Action);
		}
		OnCurrentStateUpdate.Broadcast(CompletedActions, GetCurrentTutorialActions());
		OnAnyChange.Broadcast(this);

		//If all current state actions completed enter new state
		if (AreTutorialActionsComplete(GetCurrentTutorialActions()))
		{
			OnEndState();
			SelectNewState();
		}
	}
	
	if (bAllowPreCompletion && !CompletedTutorialActions.HasTagExact(CurrentLoggedAction))
	{
		CompletedTutorialActions.AddTag(CurrentLoggedAction);
	}
}

void ATutorialManager::SelectNewState()
{
	bool bStateComplete = true;
	while (bStateComplete)
	{
		bStateComplete = false;
		TutorialState++;

		OnStartState();

		if (TutorialStateActionsPairings.IsValidIndex(TutorialState) &&
			(GetCurrentTutorialActions().Num() <= 0 || AreTutorialActionsComplete(GetCurrentTutorialActions())))
		{
			OnEndState();
			bStateComplete = true;
		}		
	}
}

void ATutorialManager::OnStartState()
{
	OnTutorialStateChange.Broadcast(GetCurrentTutorialActions(), GetStateTutorialActions(TutorialState - 1));
	OnAnyChange.Broadcast(this);
}

void ATutorialManager::OnEndState()
{
}

FGameplayTagContainer ATutorialManager::GetStateTutorialActions(int32 Index) const
{
	if (!TutorialStateActionsPairings.IsValidIndex(Index))
		return FGameplayTagContainer();
	
	return TutorialStateActionsPairings[Index].TutorialActions;
}

FGameplayTag ATutorialManager::GetStateTag(int32 Index) const
{
	if (!TutorialStateActionsPairings.IsValidIndex(Index))
		return FGameplayTag();
	
	return TutorialStateActionsPairings[Index].TutorialState;
}

ACharacter* ATutorialManager::GetPlayerPawn() const
{
	return UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ATutorialManager::EnterRegion(FGameplayTag RegionTag)
{
	CurrentRegion = RegionTag;
	OnTutorialAreaEnter.Broadcast(RegionTag);
	OnAnyChange.Broadcast(this);
}
