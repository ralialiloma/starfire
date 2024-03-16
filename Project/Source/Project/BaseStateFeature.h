// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseStateFeature.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJECT_API UBaseStateFeature : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void EnterState();

	UFUNCTION(BlueprintImplementableEvent)
	void RunFeature();

	UFUNCTION(BlueprintImplementableEvent)
	void ExitState();
	
};
