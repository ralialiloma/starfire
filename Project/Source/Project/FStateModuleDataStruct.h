// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "FStateModuleDataStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_API FStateModuleDataStruct: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	ACharacter* CharacterController;

	UPROPERTY(BlueprintReadWrite)
	bool ActionValue;

	UPROPERTY(BlueprintReadWrite)
	float ElapsedSeconds;

	UPROPERTY(BlueprintReadWrite)
	float TriggeredSeconds;
};
