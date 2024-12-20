// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "VisualFXDataAsset.generated.h"

class USoundFXProcessor;

USTRUCT(BlueprintType)
struct FVisualFXSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* SoundFile = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundAttenuation* Attenuation = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Volume = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Pitch = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartTime = 0.f;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<USoundFXProcessor*> SoundProcessors {};

	bool IsValid() const
	{
		return SoundFile != nullptr;
	}
};

UCLASS()
class STARFIRE_API UVisualFXDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	FVisualFXSettings* GetFXSettings(FGameplayTag Tag);

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (Categories = "Effects.FX.VisualFX"))
	TMap<FGameplayTag, FVisualFXSettings> FXMap;
	
};
