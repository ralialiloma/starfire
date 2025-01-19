// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitCalls.h"
#include "Components/AudioComponent.h"

void ULimitCalls::ModifyFX_Implementation(UAudioComponent*& AudioComponent) const
{
	Super::ModifyFX_Implementation(AudioComponent);


	if (CountCalls >= PlayAfterCalls)
	{
		CountCalls = 0;
		return;
	}

	CountCalls++;
	AudioComponent->DestroyComponent();
	AudioComponent = nullptr;
}
