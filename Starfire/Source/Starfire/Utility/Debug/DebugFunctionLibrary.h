// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugFunctionLibrary.generated.h"

#define SHOULD_DEBUG(Tag, DebugType) \
(UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Tag, DebugType))

#define SF_LOG(LogType, LogCategory, Text, DebugTag) \
if (SHOULD_DEBUG(DebugTag, EDebugType::Log))    \
{                                               \
UE_LOG(LogType, LogCategory, TEXT("%s"), *FString(Text)); \
}

#define SF_PRINT_TO_SCREEN(Key, DisplayTime, Color, Text, DebugTag)               \
if (SHOULD_DEBUG(DebugTag, EDebugType::Print))                                    \
{                                                                                 \
	if (GEngine)                                                                  \
	{                                                                             \
		GEngine->AddOnScreenDebugMessage(Key, DisplayTime, Color,                 \
		FString::Printf(TEXT("[%s] %s"), *Sf_GameplayTags::Debug::DebugTag.GetTag().ToString(), Text)); \
	}                                                                             \
}

#define SF_SIMPLE_DEBUG(LogType, LogCategory, Color, Text, DebugTag) \
	SF_LOG(LogType, LogCategory, Text, DebugTag) \
	SF_PRINT_TO_SCREEN(-1, 4, Color, Text, DebugTag)

UCLASS()
class STARFIRE_API UDebugFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTag, EDebugType DebugType);


	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "3", DevelopmentOnly), Category="Development")
	static void Sf_PrintString(
		UObject* WorldContextObject,
		const FString& InString= FString("Hello"),
		UPARAM(meta=(Categories="Debug"))const FGameplayTag InDebugTag = FGameplayTag(),
		const bool bPrintToScreen = true,
		const bool bPrintToLog = true,
		const FLinearColor TextColor = FLinearColor(0, 0.66, 1),
		const float Duration = 2,
		const FName Key= NAME_None);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, Keywords = "error throw exception", AdvancedDisplay = "2", DevelopmentOnly), Category="Development")
	static void Sf_ThrowError(
		const UObject* WorldContextObject,
		const FString& ErrorMessage= FString("Error"),
		const bool bPrintToScreen = true,
		const bool bPrintToLog= true);

	UFUNCTION(BlueprintCallable, Category="Rendering|Debug", meta=(WorldContext="WorldContextObject", DevelopmentOnly))
	static void Sf_DrawDebugSphere(
		const UObject* WorldContextObject,
		UPARAM(meta=(Categories="Debug"))const FGameplayTag DebugTag,
		const FVector Center,
		float Radius=100.f,
		int32 Segments=12,
		FLinearColor LineColor= FLinearColor::White,
		float Duration=0.f,
		float Thickness = 0.f);

	UFUNCTION(BlueprintCallable, Category="Rendering|Debug", meta=(WorldContext="WorldContextObject", DevelopmentOnly))
	static void Sf_DrawDebugString(
		const UObject* WorldContextObject,
		UPARAM(meta=(Categories="Debug"))const FGameplayTag DebugTag,
		const FVector TextLocation,
		const FString& Text,
		AActor* TestBaseActor = nullptr,
		FLinearColor TextColor = FLinearColor::White,
		float Duration=0.f);

	
	
};
