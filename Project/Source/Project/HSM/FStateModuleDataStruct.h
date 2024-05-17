
#pragma once
#include "CoreMinimal.h"
#include "../Utility/FunctionLibrary.h"
#include "FStateModuleDataStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECT_API FStateModuleDataStruct: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	ACharacter* CharacterController;
	
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
