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

		int RandomNumber = FMath::RandRange(0, SoundFiles.Num() - 1);

		if (RandomNumber == PreviousSoundIndex)
		{
			RandomNumber++;
			RandomNumber %= SoundFiles.Num();
		}

		PreviousSoundIndex = RandomNumber;
		return SoundFiles[RandomNumber];
	}

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DisallowSameConsecutiveSounds = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> SoundFiles {};

	UPROPERTY(Transient)
	int32 PreviousSoundIndex = -1;
};
