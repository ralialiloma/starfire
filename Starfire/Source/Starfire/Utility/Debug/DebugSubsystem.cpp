// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugSubsystem.h"
#include "DebugSettings.h"

void UDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//UE_LOG(LogTemp, Warning, TEXT("DEBUG SUBSYSTEM --- INIT"));
}

void UDebugSubsystem::Deinitialize()
{
	//UE_LOG(LogTemp, Warning, TEXT("DEBUG SUBSYSTEM --- DESTROY"));
	Super::Deinitialize();
}

bool UDebugSubsystem::ShouldDebug(FGameplayTag DebugTag, EDebugType DebugType)
{
	return GetDefault<UDebugSettings>()->ShouldDebug(DebugTag,DebugType);
}


bool UDebugSubsystem::GetAllowDebug()
{
	return !GetDefault<UDebugSettings>()->bHideAllDebugs;
}

bool UDebugSubsystem::GetMovementDebug(EDebugType DebugType)
{
	return ShouldDebug(Sf_GameplayTags::Debug::FP::Movement::Name,DebugType);
}
