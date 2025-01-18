// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Starfire/Shared/Sound/Bases/FXDataAssetBase.h"
#include "SoundFXDataAsset.generated.h"

class USoundFileType;
class USoundFXProcessor;

USTRUCT(BlueprintType)
struct FSoundFXSettings
{
	GENERATED_BODY()

	FSoundFXSettings();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	USoundFileType* SoundFileType = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundAttenuation* Attenuation = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundConcurrency* Concurrency = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Volume = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Pitch = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StartTime = 0.f;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TArray<USoundFXProcessor*> Processors {};

	bool IsValid() const;
};

DECLARE_LOG_CATEGORY_EXTERN(LogSoundFX, Log, All);

UCLASS()
class STARFIRE_API USoundFXDataAsset : public UFXDataAssetBase
{
	GENERATED_BODY()

public:
	
	FSoundFXSettings* GetSoundFXSettings(FGameplayTag Tag);

	virtual USceneComponent* ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params) override;
	virtual bool CancelFX_Implementation(USceneComponent* FXComponent) override;
	virtual float GetFXDuration_Implementation(USceneComponent* FXComponent) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (Categories = "Effects.FX.SoundFX", ForceInlineRow))
	TMap<FGameplayTag, FSoundFXSettings> FXMap;
	
};
