// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "Bases/FXHandle.h"
#include "Bases/FXParams.h"
#include "FXSubsystem.generated.h"

class UFXHandleObject;
class UFXDataAssetBase;
class UVisualFXDataAsset;
class UMessageFXPairingDataAsset;
class USoundFXDataAsset;
class USoundBase;

DECLARE_LOG_CATEGORY_EXTERN(LogFXSubsystem, Log, All);

UCLASS()
class STARFIRE_API UFXSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:

	static UFXSubsystem* Get();
	
	UFUNCTION(BlueprintCallable, Category="FXSystem", meta = (WorldContext = "WorldContext"))
	FFXHandle PlayFX(const UObject* WorldContext, UPARAM(meta=(Categories="Effects.Messages")) FGameplayTag FXMessageTag);

	UFUNCTION(BlueprintCallable, Category="FXSystem", meta = (WorldContext = "WorldContext"))
	FFXHandle PlayFXAt(const UObject* WorldContext, UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXMessageTag, FTransform Transform);

	UFUNCTION(BlueprintCallable, Category="FXSystem", meta = (AdvancedDisplay = "2", WorldContext = "WorldContext"))
	FFXHandle PlayFXOn(const UObject* WorldContext, UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXMessageTag, USceneComponent* 
	Component, FName Bone = NAME_None, FTransform Offset = FTransform());

	UFUNCTION(BlueprintCallable, Category="FXSystem")
	bool CancelFX(UPARAM(ref) FFXHandle& Handle);

protected:
	FFXHandle PlayFX_Internal(const UObject* WorldContext, FGameplayTag FXMessageTag, FFXParams Params);
	
	FGameplayTagContainer GetFXByMessageTag(FGameplayTag MessageTag) const;
	bool AllReferencesValid() const;
	virtual UWorld* GetWorld() const override;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FXSystem")
	TArray<UFXDataAssetBase*> FXDataAssets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FXSystem")
	TArray<UMessageFXPairingDataAsset*> MessageFXPairings;
	
	UPROPERTY(Transient)
	TMap<FFXHandle, TWeakObjectPtr<UFXHandleObject>> FXDataHandles {};
};
