
#pragma once
#include "CoreMinimal.h"
#include "Starfire/Utility/InputSignalType.h"
#include "StateModuleDataStruct.generated.h"

USTRUCT(BlueprintType)
struct STARFIRE_API FStateModuleDataStruct: public FTableRowBase
{
	GENERATED_BODY()

	FStateModuleDataStruct();

	FStateModuleDataStruct(ACharacter* InCharacter);

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
