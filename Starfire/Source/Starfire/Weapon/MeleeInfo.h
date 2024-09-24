#pragma once
#include "MeleeInfo.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FMeleeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FVector Extent;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	FVector Direction;

	FMeleeInfo();

	FMeleeInfo(FVector InLocation, FVector InExtent, FRotator InRotation, FVector InDirection)
	   : Location(InLocation), Extent(InExtent), Rotation(InRotation), Direction(InDirection)
	{
	}
};