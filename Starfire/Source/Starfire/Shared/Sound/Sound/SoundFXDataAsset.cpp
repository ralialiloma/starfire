// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundFXDataAsset.h"

FSoundFXSettings* USoundFXDataAsset::GetFXSettings(FGameplayTag Tag)
{
	return FXMap.Find(Tag);
}
