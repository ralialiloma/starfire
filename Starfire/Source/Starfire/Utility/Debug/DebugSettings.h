// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugType.h"
#include "Engine/DeveloperSettings.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "DebugSettings.generated.h"


#pragma region Old
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

	bool GetDebugType(const EDebugType DebugType) const
	{
		switch (DebugType)
		{
		case EDebugType::Print:
			return bPrint && bAllow;
		case  EDebugType::Log:
			return bLog && bAllow;
		case  EDebugType::Visual:
			return bVisual && bAllow;
		case  EDebugType::Sound:
			return bSound && bAllow;
		default: ;
		}

		return bAllow;
	}
};
#pragma endregion



UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "Debug Settings"))
class STARFIRE_API UDebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#pragma region Functions
	UFUNCTION(BlueprintCallable, BlueprintPure, meta =  (DeprecatedFunction))
	bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTagIn, EDebugType DebugTypeIn) const;
private:
	TArray<TMap<EDebugType, FGameplayTagContainer>> GetAllGameplayTagContainers() const;
	TArray<FGameplayTag> GetAllDebugTags(EDebugType DebugType) const;
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere)
	bool bHideAllDebugs =false;

	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere)
	bool bMuteGame =false;
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(DeprecatedFunction,ForceInlineRow,Categories="Debug.FP"))
	TMap<EDebugType,FGameplayTagContainer> FP;
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(DeprecatedFunction,ForceInlineRow,Categories="Debug.TP"))
	TMap<EDebugType,FGameplayTagContainer> TP;

	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(DeprecatedFunction,ForceInlineRow,Categories="Debug.Weapon"))
	TMap<EDebugType,FGameplayTagContainer> Weapon;
#pragma endregion

#pragma region Old
public:
	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	bool AllowDebug = false;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	bool AllowSound = false;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData AIDebug;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData WeaponDebug;

	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData HSMDebug;
	
	UPROPERTY(Config, EditAnywhere, Meta = (BlueprintBaseOnly,DeprecatedProperty), BlueprintReadOnly, Category= "Debug Settings")
	FDebugSettingData MovementDebug;
#pragma endregion
};






