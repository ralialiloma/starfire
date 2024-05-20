
#pragma once
#include "CoreMinimal.h"
#include "../Utility/FunctionLibrary.h"
#include "FStateModuleDataStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_API FStateModuleDataStruct: public FTableRowBase
{
	GENERATED_BODY()

	FStateModuleDataStruct():
	Character(nullptr),
	ActionValue_1(false),
	ActionValue_2(0),
	ElapsedSeconds(0),
	TriggeredSeconds(0),
	InputSignal()
	{
	}

	FStateModuleDataStruct(ACharacter* InCharacter):
	Character(InCharacter),
	ActionValue_1(false),
	ActionValue_2(0),
	ElapsedSeconds(0),
	TriggeredSeconds(0),
	InputSignal()
	{
	}

	UPROPERTY(BlueprintReadWrite)
	ACharacter* Character;
	
	UPROPERTY(BlueprintReadWrite)
	bool ActionValue_1;

	UPROPERTY(BlueprintReadWrite)
	float ActionValue_2;

	UPROPERTY(BlueprintReadWrite)
	FVector2D ActionValue_3;

	UPROPERTY(BlueprintReadWrite)
	float ElapsedSeconds;

	UPROPERTY(BlueprintReadWrite)
	float TriggeredSeconds;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EInputSignalType> InputSignal;
};
