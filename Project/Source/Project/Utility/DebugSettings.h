// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DebugSettings.generated.h"

UENUM()
enum EDebugType
{
	Print,
	Log,
	Visual,
	Sound
};

USTRUCT(BlueprintType)
struct FDebugSettingData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bAllow = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bPrint = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bLog = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bVisual = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bSound = true;

	bool GetDebugType(TEnumAsByte<EDebugType> DebugType) const
	{
		switch (DebugType)
		{
		case Print:
			return bPrint && bAllow;
		case Log:
			return bLog && bAllow;
		case Visual:
			return bVisual && bAllow;
		case Sound:
			return bSound && bAllow;
		default: ;
		}

		return bAllow;
	}
};

UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "Debug Settings"))
class PROJECT_API UDebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly), BlueprintReadOnly, Category= "Debug Settings")
	bool AllowDebug = false;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly), BlueprintReadOnly, Category= "Debug Settings")
	bool AllowSound = false;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData AIDebug;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData WeaponDebug;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData HSMDebug;
};
