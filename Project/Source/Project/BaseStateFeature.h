// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateFeature.generated.h"

class UStateCallstack;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJECT_API UBaseStateFeature : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Blueprintable)
	UStateCallstack* Callstack;
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void EnterState();

	UFUNCTION(BlueprintImplementableEvent)
	void RunFeature();

	UFUNCTION(BlueprintImplementableEvent)
	void ExitState();

	void SetStateCallstack(UStateCallstack* NewCallstack);
	
};
