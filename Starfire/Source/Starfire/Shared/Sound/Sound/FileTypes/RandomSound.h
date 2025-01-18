// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundFileType.h"
#include "RandomSound.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API URandomSound : public USoundFileType
{
	GENERATED_BODY()

	virtual USoundBase* GetSoundFile_Implementation() override
	{
		if (SoundFiles.Num() <= 0)
			return nullptr;

		const int RandomNumber = FMath::RandRange(0, SoundFiles.Num() - 1);
		return SoundFiles[RandomNumber];
	}

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> SoundFiles {};
};
