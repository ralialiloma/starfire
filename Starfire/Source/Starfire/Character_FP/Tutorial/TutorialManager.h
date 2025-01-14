// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/ActionLogger/ActionLogger.h"
#include "TutorialManager.generated.h"

class ATutorialArea;
class UTutorialConditions;
class ASf_FP_Character;
class ASf_BreakerPillar;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnyTutorialChange, ATutorialManager*, TutorialManager);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialAreaEnter, FGameplayTag, AreaTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentProgress, FGameplayTagContainer, CompletedActions,  FGameplayTagContainer, AllActions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTutorialProgress, FGameplayTagContainer, NewActions,  FGameplayTagContainer, PreviousActions);

USTRUCT(Blueprintable)
struct FTutorialStateActionPairing
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Tutorial.State"))
	FGameplayTag TutorialState {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTagContainer TutorialActions {};
};

UCLASS()
class STARFIRE_API ATutorialManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (Categories = "Gameplay.ActionLogger"))
	FAnyTutorialChange OnAnyChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (Categories = "Gameplay.ActionLogger"))
	FCurrentProgress OnCurrentStateUpdate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (Categories = "Gameplay.ActionLogger"))
	FTutorialProgress OnTutorialStateChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (Categories = "Gameplay.ActionLogger"))
	FTutorialAreaEnter OnTutorialAreaEnter;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSf_VoidDelegate_BP OnTutorialStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSf_VoidDelegate_BP OnTutorialEnd;


	virtual void BeginPlay() override;
	
	
	UFUNCTION(BlueprintCallable, Category = "Tutorial")
	void ToggleTutorial(FTransform InReturnTransform);
	UFUNCTION(BlueprintCallable, Category = "Tutorial")
	void StartTutorial(FTransform InReturnTransform);
	UFUNCTION(BlueprintCallable, Category = "Tutorial")
	void EndTutorial();
	UFUNCTION(BlueprintCallable, Category = "Tutorial")
	void ForceProgressTutorial();

	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	bool IsTutorialActive() const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets", meta = (Categories = "Gameplay.ActionLogger"))
	bool AreTutorialActionsComplete(FGameplayTagContainer ActionTag) const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets", meta = (Categories = "Gameplay.ActionLogger"))
	bool IsTutorialActionComplete(FGameplayTag ActionTag) const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets", meta = (Categories = "Region"))
	bool IsInRegion(FGameplayTag ActionTag) const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	int32 GetNumTutorialActionLeft() const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	int32 GetCurrentStateIndex() const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	FGameplayTag GetCurrentStateTag() const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	FGameplayTagContainer GetCurrentTutorialActions() const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets")
	FGameplayTagContainer GetAllTutorialActionsRaw() const;

	UFUNCTION(BlueprintCallable, Category = "Tutorial")
	bool EvaluateTutorialConditions(FTutorialConditionSettings Conditions);

protected:

	UFUNCTION()
	void OnActionLogged(FCachedActionLogMessage NewMessage, int MessageCount);
	UFUNCTION()
	void SelectNewState();

	UFUNCTION()
	void OnStartState();
	UFUNCTION()
	void OnEndState();

	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets", meta = (BlueprintProtected))
	FGameplayTagContainer GetStateTutorialActions(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category = "Tutorial|Gets", meta = (BlueprintProtected))
	FGameplayTag GetStateTag(int32 Index) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TutorialManager", meta = (ExposeOnSpawn))
	TArray<FTutorialStateActionPairing> TutorialStateActionsPairings {};
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "TutorialManager", meta = (ExposeOnSpawn))
	APlayerStart* TutorialStart = nullptr;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "TutorialManager", meta = (ExposeOnSpawn))
	ASf_BreakerPillar* TutorialEnd = nullptr;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "TutorialManager")
	TArray<AActor*> CrystalSpawns {};
	UPROPERTY(BlueprintReadOnly, Category = "TutorialManager", meta = (ExposeOnSpawn))
	TArray<FTransform> CrystalSpawnTransforms {};
	UPROPERTY(BlueprintReadOnly, Category = "TutorialManager")
	TArray<AActor*> EnemySpawns {};
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "TutorialManager", meta = (ExposeOnSpawn))
	TArray<FTransform> EnemySpawnTransforms {};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TutorialManager", AdvancedDisplay, meta = (ExposeOnSpawn))
	bool bAllowPreCompletion = false;

private:

	friend ATutorialArea;

	ACharacter* GetPlayerPawn() const;

	void EnterRegion(FGameplayTag RegionTag);

	UPROPERTY(Transient)
	FTransform ReturnTransform = FTransform();
	UPROPERTY(Transient)
	bool TutorialActive = false;

	UPROPERTY(Transient)
	int32 TutorialState = -1;
	UPROPERTY(Transient)
	FGameplayTagContainer CompletedTutorialActions {};
	UPROPERTY(Transient)
	FGameplayTag CurrentRegion {};
	UPROPERTY(Transient)
	UClass* EnemyClass = nullptr;
	UPROPERTY(Transient)
	UClass* CrystalClass = nullptr;
	UPROPERTY(Transient)
	TArray<AActor*> ActorsToClean {};
};
