// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

UCLASS()
class STARFIRE_API UDebugSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#pragma region Functions
	static UDebugSubsystem* GetDebugSubsystem();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTag, EDebugType DebugType);
	UFUNCTION(BlueprintCallable, BlueprintPure,	meta = (WorldContext = "WorldContext"))
	static bool GetAllowDebug();
	UFUNCTION(BlueprintCallable, BlueprintPure,	meta = (WorldContext = "WorldContext"))
	static bool GetAllowSound();
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContext"))
	static bool GetMovementDebug(EDebugType DebugType);
#pragma endregion
	
#pragma region Properties
	UPROPERTY(BlueprintReadOnly)
	bool AllowDebug = false;
	UPROPERTY(BlueprintReadOnly)
	bool AllowSound = false;
	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData AIDebug;
	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData WeaponDebug;
	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData HSMDebug;
	UPROPERTY(BlueprintReadOnly)
	FDebugSettingData MovementDebug;
#pragma endregion


	
};
