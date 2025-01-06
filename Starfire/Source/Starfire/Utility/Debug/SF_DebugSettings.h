// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "DebugVisualType.h"
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
	bool ShouldDebug(UPARAM(meta=(Categories="Debug"))FGameplayTag DebugTagIn, EDebugVisualType DebugTypeIn) const;
private:
	TArray<TMap<EDebugVisualType, FGameplayTagContainer>> GetAllGameplayTagContainers() const;
	TArray<FGameplayTag> GetAllDebugTags(EDebugVisualType DebugType) const;
	bool ShouldShowDebugType(EDebugVisualType DebugTypeIn) const;
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere)
	bool bHideAllDebugs = false;
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(ForceInlineRow,ReadOnlyKeys))
	TMap<EDebugVisualType, bool> ShownDebugTypes =
		{
		TTuple<EDebugVisualType,bool>(EDebugVisualType::Log,true),
		TTuple<EDebugVisualType,bool>(EDebugVisualType::Print,true),
		TTuple<EDebugVisualType,bool>(EDebugVisualType::Visual,true),
		TTuple<EDebugVisualType,bool>(EDebugVisualType::Sound,true),
		};
	
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere,meta=(ForceInlineRow,Categories="Debug",ReadOnlyKeys))
	TMap<EDebugVisualType,FGameplayTagContainer> ShownDebugs;
#pragma endregion
};






