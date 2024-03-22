// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "ECallIInput.h"
#include "StateCallstack.generated.h"



/**
 * 
 */
UCLASS(Blueprintable,ClassGroup=(StateMachine))
class PROJECT_API UStateCallstack : public UBetterObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UBaseState*> ActiveStatesByPriority;

public:
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn=true))
	FSoftObjectPath DataTablePath;

public:

	//Manage States
	
	UFUNCTION(BlueprintCallable)
	bool TryAddState(TSubclassOf<UBaseState> BaseStateClass, FStateModuleDataStruct Data);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveState(TSubclassOf<UBaseState> BaseStateClass, FStateModuleDataStruct Data);
	
	
	UFUNCTION(BlueprintCallable)
	void SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove,FStateModuleDataStruct Data);

	UFUNCTION(BlueprintCallable)
	TArray<UBaseState*> GetActiveStates();


	//Run Actions

	UFUNCTION(BlueprintCallable)
	void RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput,FStateModuleDataStruct Data);


	//CPP Utility Functions
	
	UBaseStateFeature* GetActiveFeature(TSubclassOf<UBaseStateFeature> FeatureClassToRun);

	void RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput,FStateModuleDataStruct Data);

	UBaseState* GetStateByClass(TSubclassOf<UBaseState> ClassToSearchFor);

private:


	
};


