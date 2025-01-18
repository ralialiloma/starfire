// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_DebugFunctionLibrary.h"
#include "SF_DebugSettings.h"
#include "Kismet/KismetSystemLibrary.h"

bool USF_DebugFunctionLibrary::ShouldDebug(const FGameplayTag DebugTag, const EDebugVisualType DebugType)
{
	return GetDefault<USF_DebugSettings>() && GetDefault<USF_DebugSettings>()->ShouldDebug(DebugTag,DebugType);
}

void USF_DebugFunctionLibrary::Sf_PrintString(
	const UObject* WorldContextObject,
	const FString& InString,
	const FGameplayTag InDebugTag,
	const bool bPrintToScreen,
	const bool bPrintToLog,
	const FLinearColor TextColor,
	const float Duration,
	const FName Key) 
{
	const FGameplayTag DebugTag = InDebugTag.IsValid() ? InDebugTag : Sf_GameplayTags::Debug::Name;
	const bool bShouldPrintToScreen = bPrintToScreen && ShouldDebug(DebugTag,EDebugVisualType::Print);
	const bool bShouldPrintToLog = bPrintToLog && ShouldDebug(DebugTag,EDebugVisualType::Log);
	if (!bShouldPrintToScreen && !bPrintToLog)
		return;

	FString DebugTagString = DebugTag.ToString();
	DebugTagString.RemoveFromStart("Debug.");
	const FString StringToPrint = "[" + DebugTagString + "] " + InString;
	
	UKismetSystemLibrary::PrintString(WorldContextObject,StringToPrint,bShouldPrintToScreen,bShouldPrintToLog,TextColor,Duration,Key);
}


void USF_DebugFunctionLibrary::Sf_ThrowError(
	const UObject* WorldContextObject,
	const FString& ErrorMessage,
	const bool bPrintToScreen,
	const bool bPrintToLog) 
{
	if (const USF_DebugSettings* Settings = GetDefault<USF_DebugSettings>())
	{
		if (Settings->bHideAllDebugs)
			return;
	}
	
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Sf_ThrowError called with invalid WorldContextObject."));
		return;
	}

	FString WorldContextObj = WorldContextObject->GetName();
	WorldContextObj.RemoveFromStart("Debug.");
	FString TimeStamp = FDateTime::Now().ToString(TEXT("%H:%M:%S"));

	const FString StringToPrint = FString::Printf(
		TEXT("[ERROR] [%s] [Context: %s] %s"),
		*TimeStamp,
		*WorldContextObj,
		*ErrorMessage
	);
	
	const FLinearColor TextColor = FLinearColor(FColor(255, 0, 0));
	constexpr float Duration = 400.f;
	UKismetSystemLibrary::PrintString(WorldContextObject,StringToPrint,bPrintToScreen,bPrintToLog,TextColor,Duration);
}

auto USF_DebugFunctionLibrary::Sf_DrawDebugSphere(
	const UObject* WorldContextObject,
	const FGameplayTag DebugTag,
	const FVector Center,
	const float Radius,
	const int32 Segments,
	const FLinearColor LineColor,
	const float Duration,
	const float Thickness) -> void
{
	if (ShouldDebug(DebugTag,EDebugVisualType::Visual))
		UKismetSystemLibrary::DrawDebugSphere(WorldContextObject,Center,Radius,Segments,LineColor,Duration,Thickness);
}

void USF_DebugFunctionLibrary::Sf_DrawDebugString(const UObject* WorldContextObject, const FGameplayTag DebugTag, const FVector TextLocation,
	const FString& Text, AActor* TestBaseActor, const FLinearColor TextColor, const float Duration)
{
	if (ShouldDebug(DebugTag,EDebugVisualType::Visual))
		UKismetSystemLibrary::DrawDebugString(WorldContextObject,TextLocation,Text,TestBaseActor,TextColor,Duration);
}

