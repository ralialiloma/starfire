// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EInputSignalType
{
	Triggered,
	Started,
	Ongoing,
	Canceled,
	Completed
};

UCLASS()
class PROJECT_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	
	public:
		UFUNCTION(BlueprintCallable,meta  = (ExpandEnumAsExecs = "SignalType"))
		static EInputSignalType ConvertToInputSignalType(EInputSignalType SignalType);
	
};
