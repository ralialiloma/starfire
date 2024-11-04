// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugFunctionLibrary.h"
#include "DebugSettings.h"
#include "Kismet/KismetSystemLibrary.h"

bool UDebugFunctionLibrary::ShouldDebug(const FGameplayTag DebugTag, const EDebugType DebugType)
{
	return GetDefault<UDebugSettings>() && GetDefault<UDebugSettings>()->ShouldDebug(DebugTag,DebugType);
}

void UDebugFunctionLibrary::Sf_PrintString(
	UObject* WorldContextObject,
	const FString& InString,
	const FGameplayTag InDebugTag,
	const bool bPrintToScreen,
	const bool bPrintToLog,
	const FLinearColor TextColor,
	const float Duration,
	const FName Key) 
{
	const FGameplayTag DebugTag = InDebugTag.IsValid() ? InDebugTag : Sf_GameplayTags::Debug::Name;
	const bool bShouldPrintToScreen = bPrintToScreen && ShouldDebug(DebugTag,EDebugType::Print);
	const bool bShouldPrintToLog = bPrintToLog && ShouldDebug(DebugTag,EDebugType::Log);
	if (!bShouldPrintToScreen && !bPrintToLog)
		return;

	FString DebugTagString = DebugTag.ToString();
	DebugTagString.RemoveFromStart("Debug.");
	const FString StringToPrint = "[" + DebugTagString + "] " + InString;
	
	UKismetSystemLibrary::PrintString(WorldContextObject,StringToPrint,bShouldPrintToScreen,bShouldPrintToLog,TextColor,Duration,Key);
}

auto UDebugFunctionLibrary::Sf_DrawDebugSphere(
	const UObject* WorldContextObject,
	const FGameplayTag DebugTag,
	const FVector Center,
	const float Radius,
	const int32 Segments,
	const FLinearColor LineColor,
	const float Duration,
	const float Thickness) -> void
{
	if (ShouldDebug(DebugTag,EDebugType::Visual))
		UKismetSystemLibrary::DrawDebugSphere(WorldContextObject,Center,Radius,Segments,LineColor,Duration,Thickness);
}

void UDebugFunctionLibrary::Sf_DrawDebugString(const UObject* WorldContextObject, const FGameplayTag DebugTag, const FVector TextLocation,
	const FString& Text, AActor* TestBaseActor, const FLinearColor TextColor, const float Duration)
{
	if (ShouldDebug(DebugTag,EDebugType::Visual))
		UKismetSystemLibrary::DrawDebugString(WorldContextObject,TextLocation,Text,TestBaseActor,TextColor,Duration);
}

