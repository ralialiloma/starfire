#pragma once
#include "Components/BoxComponent.h"
#include "MeleeInfo.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FMeleeInfo
{
	GENERATED_BODY()

	/*UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FVector Extent;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	FVector Direction;;*/

	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	FMeleeInfo();
	FMeleeInfo(UBoxComponent* BoxComponentIn);
};