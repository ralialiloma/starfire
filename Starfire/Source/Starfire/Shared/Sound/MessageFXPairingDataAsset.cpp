// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageFXPairingDataAsset.h"

FGameplayTagContainer UMessageFXPairingDataAsset::GetMappedFX(FGameplayTag Tag)
{
	if (FMessageFXPairings* FoundPair = MappedFXs.Find(Tag))
		return FoundPair->Effects;

	return FGameplayTagContainer();
}
