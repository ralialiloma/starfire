﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Starfire/Shared/Sound/FXDataAssetBase.h"
#include "VisualFXDataAsset.generated.h"

class UVisualFXProcessor;
class USoundFXProcessor;

USTRUCT(BlueprintType)
struct FVisualFXSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* ParticleSystem = nullptr;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<UVisualFXProcessor*> Processors {};

	bool IsValid() const
	{
		return ParticleSystem != nullptr;
	}
};

UCLASS()
class STARFIRE_API UVisualFXDataAsset : public UFXDataAssetBase
{
	GENERATED_BODY()

public:
	
	FVisualFXSettings* GetFXSettings(FGameplayTag Tag);

	virtual void ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (Categories = "Effects.FX.VisualFX", ForceInlineRow))
	TMap<FGameplayTag, FVisualFXSettings> FXMap;
	
};
