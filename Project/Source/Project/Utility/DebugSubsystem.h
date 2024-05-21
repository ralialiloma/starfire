// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

UCLASS()
class PROJECT_API UDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	static UDebugSubsystem* GetDebugSubsystem(const UObject* WorldContext);
	
	UPROPERTY(BlueprintReadOnly)
	bool AllowDebug = false;
	UFUNCTION(BlueprintCallable, BlueprintPure,	meta = (WorldContext = "WorldContext"))
	static bool GetAllowDebug(const UObject* WorldContext);

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData AIDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetAIDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType);

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData WeaponDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetWeaponDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType);

	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData HSMDebug;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetHSMDebug(const UObject* WorldContext, TEnumAsByte<EDebugType> DebugType);
	
};
