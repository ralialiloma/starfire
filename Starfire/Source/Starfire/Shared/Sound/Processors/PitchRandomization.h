// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/Sound/SoundProcessor.h"
#include "PitchRandomization.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UPitchRandomization : public USoundProcessor
{
	GENERATED_BODY()

public:
	virtual void ModifyAudio_Implementation(UAudioComponent* AudioComponent) const override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinPitch = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxPitch = 1.5;
};
