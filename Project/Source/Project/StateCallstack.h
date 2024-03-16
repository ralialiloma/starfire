// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include <functional>
#include "UObject/NoExportTypes.h"
#include "StateCallstack.generated.h"

UENUM()
enum ECallInput
{
	Enter, Run, Exit
};


/**
 * 
 */
UCLASS()
class PROJECT_API UStateCallstack : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UBaseState*> ActiveStatesByPriority;

public:
	UFUNCTION(BlueprintCallable)
	bool TryAddActiveStates(TSubclassOf<UBaseState> BaseStateClass);
	UFUNCTION(BlueprintCallable)
	void RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput callInput);

private:


	
};


