// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundDataAsset.h"

FSoundSettings* USoundDataAsset::GetSoundSettings(FGameplayTag Tag)
{
	return SoundMap.Find(Tag);
}
