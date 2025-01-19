// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayProbability.h"

#include "Components/AudioComponent.h"

void UPlayProbability::ModifyFX_Implementation(UAudioComponent*& AudioComponent) const
{
	Super::ModifyFX_Implementation(AudioComponent);

	if (DisallowConsecutiveCalls && HasPlayed)
	{
		HasPlayed = false;
		return;
	}

	if (Probability.Evaluate())
	{
		HasPlayed = true;
		return;
	}
	
	AudioComponent->DestroyComponent();
	AudioComponent = nullptr;
}
