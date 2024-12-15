// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "SoundSubsystem.generated.h"

class USoundDataAsset;
class USoundBase;

UCLASS()
class STARFIRE_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SoundSystem")
	void PlaySound(FGameplayTag SoundTag);

	UFUNCTION(BlueprintCallable, Category="SoundSystem"/*, meta = (AdvancedDisplay = "1")*/)
	void PlaySoundAt(FGameplayTag SoundTag, FVector Location);

	UFUNCTION(BlueprintCallable, Category="SoundSystem")
	void PlaySoundOn(FGameplayTag SoundTag, USceneComponent* Component, FName Bone = NAME_None, FVector Offset = 
	FVector::ZeroVector);

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoundSystem")
	USoundDataAsset* SoundDataAsset;
};
