// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FXSystemSettings.generated.h"

class UMessageFXPairingDataAsset;
class UVisualFXDataAsset;
class USoundFXDataAsset;


UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="FX System"))
class STARFIRE_API UFXSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category="Sound System")
	TSoftObjectPtr<USoundFXDataAsset> DefaultSoundFXDataAsset;
	UPROPERTY(EditAnywhere, Config, Category="Sound System")
	TSoftObjectPtr<UVisualFXDataAsset> DefaultVisualFXDataAsset;
	UPROPERTY(EditAnywhere, Config, Category="Sound System")
	TSoftObjectPtr<UMessageFXPairingDataAsset> MessageFXPairings;
};
