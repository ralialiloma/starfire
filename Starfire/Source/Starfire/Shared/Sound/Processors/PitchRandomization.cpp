// Fill out your copyright notice in the Description page of Project Settings.


#include "PitchRandomization.h"

#include "Components/AudioComponent.h"

void UPitchRandomization::ModifyAudio_Implementation(UAudioComponent* AudioComponent) const
{
	Super::ModifyAudio_Implementation(AudioComponent);

	AudioComponent->SetPitchMultiplier(FMath::RandRange(MinPitch, MaxPitch));
}
