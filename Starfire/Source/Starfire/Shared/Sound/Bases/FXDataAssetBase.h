// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FXParams.h"
#include "FXDataAssetBase.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class STARFIRE_API UFXDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (WorldContext = "WorldContext"))
	USceneComponent* ExecuteFX(UObject* WorldContext, FFXParams Params);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (WorldContext = "WorldContext"))
	bool CancelFX(USceneComponent* FXComponent);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (WorldContext = "WorldContext"))
	float GetFXDuration(USceneComponent* FXComponent);
};
