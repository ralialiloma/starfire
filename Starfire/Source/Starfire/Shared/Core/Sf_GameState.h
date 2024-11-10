// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameStateBase.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Sf_GameState.generated.h"

class ACheckPoint;
DECLARE_LOG_CATEGORY_EXTERN(LogSFGameState, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayStateChange, FGameplayTag, NewState, FGameplayTag, OldState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCheckpointChange, FTransform, NewCheckpoint, FTransform, OldCheckpoint);

UCLASS(Abstract)
class STARFIRE_API ASf_GameState : public AGameStateBase
{
	GENERATED_BODY()

#pragma region Checkpoints
public:
	UPROPERTY(BlueprintAssignable)
	FCheckpointChange OnCheckPointChange;
	
	UFUNCTION(BlueprintCallable)
	bool SetCheckpoint(ACheckPoint* NewCheckpoint);
	UFUNCTION(BlueprintCallable)
	ACheckPoint* GetLastCheckpoint() const;
	UFUNCTION(BlueprintCallable)
	bool GetLastCheckpointTransform(FTransform& ReturnTransform) const;

protected:
	UPROPERTY(SaveGame)
	FTransform SavedCheckPointTransform = FTransform();
	UPROPERTY(SaveGame)
	bool bHasCheckPointed = false;
	UPROPERTY(Transient)
	ACheckPoint* CurrentCheckPoint = nullptr;

#pragma endregion

#pragma region PlayState

public:

	UPROPERTY(BlueprintAssignable, Category = "PlayState")
	FPlayStateChange OnPlayStateChange;

	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (Categories = "Gameplay.PlayState"))
	bool SetPlayState(FGameplayTag NewPlayState);
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void EndGame(bool HasWon);

	UFUNCTION(BlueprintCallable, Category = "PlayState")
	FGameplayTag GetPlayState() const;
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	bool HasWonGame() const;
	UFUNCTION(BlueprintCallable, Category = "PlayState", meta = (Categories = "Gameplay.PlayState"))
	bool IsInPlayState(FGameplayTag NewPlayState) const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayState", meta = (Categories = "Gameplay.PlayState"))
	FGameplayTag CurrentPlayState = Sf_GameplayTags::Gameplay::PlayState::Tutorial;

	UPROPERTY()
	bool bHasWon = false;

#pragma endregion
	
};
