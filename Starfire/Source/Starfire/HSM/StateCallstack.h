// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "ECallIInput.h"
#include "Starfire/Utility/BetterObject.h"
#include "InputAction.h"
#include "StateCallstack.generated.h"



/**
 * 
 */
UCLASS(Blueprintable,ClassGroup=(StateMachine))
class STARFIRE_API UStateCallstack : public UBetterObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UBaseState*> ActiveStatesByPriority = TArray<UBaseState*>{};
	
	UPROPERTY()
	TArray<UBaseStateFeature*> CurrentActiveFeatures  = TArray<UBaseStateFeature*>{};


public:
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn=true),Category = "StateMachine")
	FSoftObjectPath StateDefintionDT;

	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn=true),Category = "StateMachine")
	ACharacter* OwningCharacter;

public:

	//Manage States
	UFUNCTION(BlueprintCallable)
	bool TryAddState(TSubclassOf<UBaseState> BaseStateClass);

	UFUNCTION(BlueprintCallable)
	void NotifyFeatures();

	UFUNCTION(BlueprintCallable)
	bool TryRemoveState(TSubclassOf<UBaseState> BaseStateClass);

	UFUNCTION(BlueprintCallable)
	bool ToggleStateByBool(TSubclassOf<UBaseState> BaseStateClass, bool bValue);
	
	UFUNCTION(BlueprintCallable)
	void SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove);

	UFUNCTION(BlueprintCallable)
	TArray<UBaseState*> GetAllActiveStates() const;

	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<UBaseStateFeature>> GetAllCurrentFeatureTypes();

	UFUNCTION(BlueprintCallable)
	TArray<UBaseStateFeature*> GetAllActiveFeatures();
	
	//Run Actions
	UFUNCTION(BlueprintCallable)
	void RunCallStackByFeature(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput,FStateModuleDataStruct Data);

	UFUNCTION(BlueprintCallable)
	void RunCallStackByInputAction(UInputAction* InputAction, ECallInput CallInput,FStateModuleDataStruct Data);

	//CPP Utility Functions
	UBaseStateFeature* GetActiveFeature(TSubclassOf<UBaseStateFeature> FeatureClassToRun);

	void RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput, const FStateModuleDataStruct& Data);

	UBaseState* GetStateByClass(TSubclassOf<UBaseState> ClassToSearchFor);

private:


	
};


