// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/Sound/Sound/SoundFXProcessor.h"
#include "PitchRandomization.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UPitchRandomization : public USoundFXProcessor
{
	GENERATED_BODY()

public:
	virtual void ModifyFX_Implementation(UAudioComponent* AudioComponent) const override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinPitch = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxPitch = 1.5;
};
