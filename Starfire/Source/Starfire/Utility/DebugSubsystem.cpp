// Fill out your copyright notice in the Description page of Project Settings.


#include "../Utility/DebugSubsystem.h"

#include "DebugSettings.h"

void UDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("DEBUGSUBSYSTEM --- INIT"));
}

void UDebugSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("DEBUGSUBSYSTEM --- DESTROY"));
	
	Super::Deinitialize();
}

UDebugSubsystem* UDebugSubsystem::Refresh()
{
	const UDebugSettings* Settings = GetDefault<UDebugSettings>();

	AllowDebug = Settings->AllowDebug;
	AllowSound = Settings->AllowSound;
	AIDebug = Settings->AIDebug;
	WeaponDebug = Settings->WeaponDebug;
	HSMDebug = Settings->HSMDebug;
	MovementDebug = Settings->MovementDebug ;

	return this;
}

UDebugSubsystem* UDebugSubsystem::GetDebugSubsystem()
{
	return GEngine->GetEngineSubsystem<UDebugSubsystem>()->Refresh();
}

bool UDebugSubsystem::GetAllowDebug()
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetAllowSound()
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->AllowSound;
	return true;
}

bool UDebugSubsystem::GetAIDebug(TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->AIDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetWeaponDebug(TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->WeaponDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetHSMDebug(TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->HSMDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}

bool UDebugSubsystem::GetMovementDebug(TEnumAsByte<EDebugType> DebugType)
{
	if (UDebugSubsystem* Subsystem = GetDebugSubsystem())
		return Subsystem->MovementDebug.GetDebugType(DebugType) && Subsystem->AllowDebug;
	return true;
}
