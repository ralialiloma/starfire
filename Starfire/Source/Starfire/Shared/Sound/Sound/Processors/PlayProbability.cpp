// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayProbability.h"

#include "Components/AudioComponent.h"

void UPlayProbability::ModifyFX_Implementation(UAudioComponent*& AudioComponent) const
{
	Super::ModifyFX_Implementation(AudioComponent);

	bool ShouldFail = DisallowConsecutiveCalls && HasPlayed;
	if (Probability.Evaluate() && !ShouldFail)
	{
		HasPlayed = true;
		CountFailedCalls = 0;
		return;
	}

	HasPlayed = false;
	CountFailedCalls++;
	AudioComponent->DestroyComponent();
	AudioComponent = nullptr;
}
