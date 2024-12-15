// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SoundSystemSettings.generated.h"

class USoundDataAsset;


UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Sound System"))
class STARFIRE_API USoundSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category="Sound System")
	TSoftObjectPtr<USoundDataAsset> DefaultSoundDataAsset;
};
