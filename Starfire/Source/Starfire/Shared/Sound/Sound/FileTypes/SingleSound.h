// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundFileType.h"
#include "SingleSound.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API USingleSound : public USoundFileType
{
	GENERATED_BODY()

	virtual bool IsValid_Implementation() override
	{
		return SoundFile != nullptr;
	}

	virtual USoundBase* GetSoundFile_Implementation() override
	{
		return SoundFile;
	}

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* SoundFile = nullptr;
};
