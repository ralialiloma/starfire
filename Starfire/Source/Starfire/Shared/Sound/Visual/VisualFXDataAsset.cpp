// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualFXDataAsset.h"

FVisualFXSettings* UVisualFXDataAsset::GetFXSettings(FGameplayTag Tag)
{
	return FXMap.Find(Tag);
}

void UVisualFXDataAsset::ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params)
{
	
}