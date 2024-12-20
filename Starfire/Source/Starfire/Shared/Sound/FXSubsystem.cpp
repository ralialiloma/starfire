// Fill out your copyright notice in the Description page of Project Settings.


#include "FXSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "FXSystemSettings.h"
#include "MessageFXPairingDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundFXDataAsset.h"
#include "Sound/SoundFXProcessor.h"

void UFXSubsystem::PlayFX(FGameplayTag FXTag)
{
	if (!SoundFXDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}

	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);
	for (auto Tag : FXTags)
	{
		if (FSoundFXSettings* FoundSound = SoundFXDataAsset->GetFXSettings(Tag))
		{
			if (!FoundSound->IsValid())
			{
				UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *FXTag.ToString()));
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
			
				Processor->ModifyFX(AudioComp);
			}
		}
		else
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *FXTag.ToString()));
		}
		//TODO: VFX
	}
}

void UFXSubsystem::PlayFXAt(FGameplayTag FXTag, FVector Location)
{
	if (!SoundFXDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}

	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);
	for (auto Tag : FXTags)
	{
		if (FSoundFXSettings* FoundSound = SoundFXDataAsset->GetFXSettings(Tag))
		{
			if (!FoundSound->IsValid())
			{
				UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *FXTag.ToString()));
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
			
				Processor->ModifyFX(AudioComp);
			}
		}
		else
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *FXTag.ToString()));
		}
	}
}

void UFXSubsystem::PlayFXOn(FGameplayTag FXTag, USceneComponent* Component, FName Bone, FVector Offset)
{
	if (!SoundFXDataAsset)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No SoundDataAsset set on SoundSystemSubsystem.")));
		return;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return;
	}
	
	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);
	for (auto Tag : FXTags)
	{
		if (FSoundFXSettings* FoundSound = SoundFXDataAsset->GetFXSettings(Tag))
		{
			if (!FoundSound->IsValid())
			{
				UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *FXTag.ToString()));
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
				
				Processor->ModifyFX(AudioComp);
			}
		}
		else
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No sound found for GameplayTag: %s"), *FXTag.ToString()));
		}
		//TODO: VFX
	}
}

FGameplayTagContainer UFXSubsystem::GetFXByMessageTag(FGameplayTag MessageTag) const
{
	if (MessageFXPairings)
		return MessageFXPairings->GetMappedFX(MessageTag);
	
	return FGameplayTagContainer();
}

void UFXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UFXSystemSettings* Settings = GetDefault<UFXSystemSettings>();
	if (Settings && Settings->DefaultSoundFXDataAsset.IsValid())
	{
		SoundFXDataAsset = Settings->DefaultSoundFXDataAsset.LoadSynchronous();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No SoundDataAsset assigned in Project Settings > Project > Sound System"));
	}
}
