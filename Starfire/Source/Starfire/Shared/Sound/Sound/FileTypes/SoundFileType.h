// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoundFileType.generated.h"

/**
 * 
 */
UCLASS(BlueprintType ,Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class STARFIRE_API USoundFileType : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USoundBase* GetSoundFile();
};
