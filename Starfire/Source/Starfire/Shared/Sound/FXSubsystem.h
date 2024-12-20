// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "FXSubsystem.generated.h"

class UVisualFXDataAsset;
class UMessageFXPairingDataAsset;
class USoundFXDataAsset;
class USoundBase;

UCLASS()
class STARFIRE_API UFXSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SoundSystem")
	void PlayFX(UPARAM(meta=(Categories="Effects.Messages")) FGameplayTag FXTag);

	UFUNCTION(BlueprintCallable, Category="SoundSystem")
	void PlayFXAt(UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXTag, FVector Location);

	UFUNCTION(BlueprintCallable, Category="SoundSystem", meta = (AdvancedDisplay = "2"))
	void PlayFXOn(UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXTag, USceneComponent* Component, FName Bone = NAME_None, FVector Offset = FVector::ZeroVector);

protected:

	FGameplayTagContainer GetFXByMessageTag(FGameplayTag MessageTag) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoundSystem")
	USoundFXDataAsset* SoundFXDataAsset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoundSystem")
	UVisualFXDataAsset* VisualFXDataAsset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoundSystem")
	UMessageFXPairingDataAsset* MessageFXPairings;
};
