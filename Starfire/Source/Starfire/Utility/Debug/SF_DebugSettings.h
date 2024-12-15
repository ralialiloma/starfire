// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "DebugType.h"
#include "Engine/DeveloperSettings.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"
#include "SF_DebugSettings.generated.h"

UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "Debug Settings"))
class STARFIRE_API USF_DebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTagIn, EDebugType DebugTypeIn) const;
private:
	TArray<TMap<EDebugType, FGameplayTagContainer>> GetAllGameplayTagContainers() const;
	TArray<FGameplayTag> GetAllDebugTags(EDebugType DebugType) const;
	bool ShouldShowDebugType(EDebugType DebugTypeIn) const;
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere)
	bool bHideAllDebugs = false;
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(ForceInlineRow,ReadOnlyKeys))
	TMap<EDebugType, bool> ShownDebugTypes =
		{
		TTuple<EDebugType,bool>(EDebugType::Log,true),
		TTuple<EDebugType,bool>(EDebugType::Print,true),
		TTuple<EDebugType,bool>(EDebugType::Visual,true),
		TTuple<EDebugType,bool>(EDebugType::Sound,true),
		};
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(ForceInlineRow,Categories="Debug",ReadOnlyKeys))
	TMap<EDebugType,FGameplayTagContainer> ShownDebugs;
#pragma endregion
};






