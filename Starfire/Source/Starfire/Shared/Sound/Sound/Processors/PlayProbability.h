// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/Sound/Sound/SoundFXProcessor.h"
#include "Structs/Probability.h"
#include "PlayProbability.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UPlayProbability : public USoundFXProcessor
{
	GENERATED_BODY()

public:
	virtual void ModifyFX_Implementation(UAudioComponent*& AudioComponent) const override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FProbability Probability = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DisallowConsecutiveCalls = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bMaxFailedCalls = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bMaxFailedCalls))
	int32 MaxFailedCalls = 5;

	UPROPERTY(Transient)
	mutable bool HasPlayed = false;
	UPROPERTY(Transient)
	mutable int32 CountFailedCalls = false;
};
