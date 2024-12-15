// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_GameState.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/Respawn/CheckPoint.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogSFGameState);

bool ASf_GameState::SetCheckpoint(ACheckPoint* NewCheckpoint)
{
	if (!NewCheckpoint)
		return false;

	FTransform OldTransform = SavedCheckPointTransform;
	CurrentCheckPoint = NewCheckpoint;
	SavedCheckPointTransform = CurrentCheckPoint->GetTransform();
	bHasCheckPointed = true;
	OnCheckPointChange.Broadcast(SavedCheckPointTransform, OldTransform);
	return true;
}

ACheckPoint* ASf_GameState::GetLastCheckpoint() const
{
	return CurrentCheckPoint;
}

bool ASf_GameState::GetLastCheckpointTransform(FTransform& ReturnTransform) const
{
	ReturnTransform = SavedCheckPointTransform;
	return bHasCheckPointed;
}

bool ASf_GameState::SetPlayState(FGameplayTag NewPlayState)
{
	if (!NewPlayState.MatchesTag(Sf_GameplayTags::Gameplay::PlayState::Name) || NewPlayState.MatchesTagExact(Sf_GameplayTags::Gameplay::PlayState::Name))
	{
		SF_SIMPLE_DEBUG(
			LogSFGameState,
			Error,
			FColor::Red,
			*FString("Trying to set new play state to not supported PlayState tag! - " + NewPlayState.ToString()),
			Core::GameState);
		return false;
	}

	FGameplayTag OldPlayState = CurrentPlayState;
	CurrentPlayState = NewPlayState;

	OnChangeState(NewPlayState, OldPlayState);
	OnPlayStateChange.Broadcast(NewPlayState, OldPlayState);
	return true;
}

FGameplayTag ASf_GameState::GetPlayState() const
{
	return CurrentPlayState;
}

bool ASf_GameState::HasWonGame() const
{
	return bHasWon;
}

bool ASf_GameState::IsInPlayState(FGameplayTag NewPlayState) const
{
	return CurrentPlayState.MatchesTag(NewPlayState);
}

void ASf_GameState::OnChangeState_Implementation(FGameplayTag NewState, FGameplayTag OldState)
{
	
}

void ASf_GameState::EndGame(bool HasWon)
{
	bHasWon = HasWon;
	SetPlayState(Sf_GameplayTags::Gameplay::PlayState::End);
	SF_SIMPLE_DEBUG(LogSFGameState, Log, FColor::Blue, *FString("Game Ended!"), Core::GameState);
}
