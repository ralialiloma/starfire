// Fill out your copyright notice in the Description page of Project Settings.


#include "PitchRandomization.h"

#include "Components/AudioComponent.h"

void UPitchRandomization::ModifyFX_Implementation(UAudioComponent*& AudioComponent) const
{
	Super::ModifyFX_Implementation(AudioComponent);

	float CurrentPitch = FMath::RandRange(MinPitch, MaxPitch);

	if (DisallowSimilarPitchConsecutively)
	{
		float PitchMax = PreviousPitch + ConsecutivePitchTolerance;
		float PitchMin = PreviousPitch - ConsecutivePitchTolerance;
		if ((CurrentPitch < PitchMax) && (CurrentPitch > PitchMin) )
		{
			bool bLower =  FMath::Abs(PitchMin - CurrentPitch) < FMath::Abs(PitchMax - CurrentPitch);
			CurrentPitch += ConsecutivePitchTolerance * (bLower ? -1 : 1);
			CurrentPitch = FMath::Clamp(CurrentPitch, MinPitch, MaxPitch);
		}
	}
	
	AudioComponent->SetPitchMultiplier(CurrentPitch);
	PreviousPitch = CurrentPitch;
}
