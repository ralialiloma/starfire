// Fill out your copyright notice in the Description page of Project Settings.


#include "FXDataAssetBase.h"

USceneComponent* UFXDataAssetBase::ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params)
{
	return nullptr;
}

bool UFXDataAssetBase::CancelFX_Implementation(USceneComponent* FXComponent)
{
	return false;
}

float UFXDataAssetBase::GetFXDuration_Implementation(USceneComponent* FXComponent)
{
	return 0.f;
}
