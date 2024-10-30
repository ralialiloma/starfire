// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugFunctionLibrary.generated.h"

#define SHOULD_DEBUG(Tag, DebugType) \
(UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Tag, DebugType))

UCLASS()
class STARFIRE_API UDebugFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTag, EDebugType DebugType);
};
