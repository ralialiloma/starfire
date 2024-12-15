// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "SoundDataAsset.h"
#include "SoundProcessor.h"
#include "SoundSystemSettings.h"
#include "Kismet/GameplayStatics.h"

void USoundSubsystem::PlaySound(FGameplayTag SoundTag)
{
	if (!SoundDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}
	
	if (FSoundSettings* FoundSound = SoundDataAsset->GetSoundSettings(SoundTag))
	{
		if (!FoundSound->IsValid())
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *SoundTag.ToString()));
			return;
		}
	
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(
			GetWorld(),
			FoundSound->SoundFile,
			FoundSound->Volume,
			FoundSound->Pitch,
			FoundSound->StartTime);

		for (auto Processor : FoundSound->SoundProcessors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyAudio(AudioComp);
		}
	}
	else
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *SoundTag.ToString()));
	}
}

void USoundSubsystem::PlaySoundAt(FGameplayTag SoundTag, FVector Location)
{
	if (!SoundDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}
	
	if (FSoundSettings* FoundSound = SoundDataAsset->GetSoundSettings(SoundTag))
	{
		if (!FoundSound->IsValid())
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *SoundTag.ToString()));
			return;
		}
	
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
			FoundSound->SoundFile,
			Location,
			FRotator::ZeroRotator, 
			FoundSound->Volume,
			FoundSound->Pitch,
			FoundSound->StartTime,
			FoundSound->Attenuation);

		for (auto Processor : FoundSound->SoundProcessors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyAudio(AudioComp);
		}
	}
	else
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *SoundTag.ToString()));
	}
}

void USoundSubsystem::PlaySoundOn(FGameplayTag SoundTag, USceneComponent* Component, FName Bone, FVector Offset)
{
	if (!SoundDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}
	
	if (FSoundSettings* FoundSound = SoundDataAsset->GetSoundSettings(SoundTag))
	{
		if (!FoundSound->IsValid())
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *SoundTag.ToString()));
			return;
		}
	
		UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAttached(
			FoundSound->SoundFile,
			Component,
			Bone,
			Offset,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			false,
			FoundSound->Volume,
			FoundSound->Pitch,
			FoundSound->StartTime,
			FoundSound->Attenuation);

		for (auto Processor : FoundSound->SoundProcessors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyAudio(AudioComp);
		}
	}
	else
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *SoundTag.ToString()));
	}
}

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USoundSystemSettings* Settings = GetDefault<USoundSystemSettings>();
	if (Settings && Settings->DefaultSoundDataAsset.IsValid())
	{
		SoundDataAsset = Settings->DefaultSoundDataAsset.LoadSynchronous();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No SoundDataAsset assigned in Project Settings > Project > Sound System"));
	}
}
