// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

UCLASS()
class PROJECT_API UDebugSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UDebugSubsystem* Refresh();
	
	static UDebugSubsystem* GetDebugSubsystem();
	
	UPROPERTY(BlueprintReadOnly)
	bool AllowDebug = false;
	UFUNCTION(BlueprintCallable, BlueprintPure,	meta = (WorldContext = "WorldContext"))
	static bool GetAllowDebug();
	
	UPROPERTY(BlueprintReadOnly)
	bool AllowSound = false;
	UFUNCTION(BlueprintCallable, BlueprintPure,	meta = (WorldContext = "WorldContext"))
	static bool GetAllowSound();

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData AIDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetAIDebug(TEnumAsByte<EDebugType> DebugType);

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData WeaponDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetWeaponDebug(TEnumAsByte<EDebugType> DebugType);

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData HSMDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetHSMDebug(TEnumAsByte<EDebugType> DebugType);
	
};
