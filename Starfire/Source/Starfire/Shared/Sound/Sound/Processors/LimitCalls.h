// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/Sound/Sound/SoundFXProcessor.h"
#include "Structs/Probability.h"
#include "LimitCalls.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API ULimitCalls : public USoundFXProcessor
{
	GENERATED_BODY()

public:
	virtual void ModifyFX_Implementation(UAudioComponent*& AudioComponent) const override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayAfterCalls = 5;
	
	UPROPERTY(Transient)
	mutable int32 CountCalls = false;
};
