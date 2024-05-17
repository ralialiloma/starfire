// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include  "ECallIInput.h"
#include "FStateModuleDataStruct.h"
#include "Project/Utility/BetterObject.h"
#include "BaseStateFeature.generated.h"

class UBaseState;
class UStateCallstack;


UCLASS(Abstract, Blueprintable)
class PROJECT_API UBaseStateFeature : public UBetterObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UStateCallstack* Callstack;
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void EnterState();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RunFeature(FStateModuleDataStruct Data);

	UFUNCTION(BlueprintImplementableEvent)
	void ExitState();

	void RunAction(ECallInput CallInput,FStateModuleDataStruct Data);

	void Initialize(UStateCallstack* NewCallstack);
	
};
