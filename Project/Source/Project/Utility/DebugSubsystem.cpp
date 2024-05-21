// Fill out your copyright notice in the Description page of Project Settings.


#include "../Utility/DebugSubsystem.h"

#include "DebugSettings.h"

void UDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDebugSettings* Settings = GetDefault<UDebugSettings>();

	AllowDebug = Settings->AllowDebug;
	AIDebug = Settings->AIDebug;
	WeaponDebug = Settings->WeaponDebug;
	HSMDebug = Settings->HSMDebug;
}

UDebugSubsystem* UDebugSubsystem::GetDebugSubsystem(const UObject* WorldContext)
{
	if (auto World = WorldContext->GetWorld())
	{
		if (auto GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UDebugSubsystem>();
		}
	}
	return nullptr;
}

bool UDebugSubsystem::GetAllowDebug(const UObject* WorldContext)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem(WorldContext))
		return Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetAIDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem(WorldContext))
		return Subsystem->AIDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetWeaponDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem(WorldContext))
		return Subsystem->WeaponDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetHSMDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem(WorldContext))
		return Subsystem->HSMDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}
