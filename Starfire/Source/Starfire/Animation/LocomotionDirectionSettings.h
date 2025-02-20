#pragma once
#include "LocomotionDirectionSettings.generated.h"

USTRUCT(BlueprintType)
struct FLocomotionDirectionSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float FMin;
	UPROPERTY(BlueprintReadWrite)
	float FMax;
	UPROPERTY(BlueprintReadWrite)
	float BMin;
	UPROPERTY(BlueprintReadWrite)
	float BMax;
	UPROPERTY(BlueprintReadWrite)
	float DeadZone;
	
};
