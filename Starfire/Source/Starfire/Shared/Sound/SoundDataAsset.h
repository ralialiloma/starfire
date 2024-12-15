// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SoundDataAsset.generated.h"

class USoundProcessor;

USTRUCT(BlueprintType)
struct FSoundSettings
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
	TArray<USoundProcessor*> SoundProcessors {};

	bool IsValid() const
	{
		return SoundFile != nullptr;
	}
};

UCLASS()
class STARFIRE_API USoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	FSoundSettings* GetSoundSettings(FGameplayTag Tag);

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<FGameplayTag, FSoundSettings> SoundMap;
	
};
