// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundFXDataAsset.h"

#include "DebugFunctionLibrary.h"
#include "SoundFXProcessor.h"
#include "Components/AudioComponent.h"
#include "FileTypes/SingleSound.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogSoundFX);

FSoundFXSettings::FSoundFXSettings()
{
	SoundFileType = NewObject<USingleSound>();
}

bool FSoundFXSettings::IsValid() const
{
	return SoundFileType && SoundFileType->GetSoundFile() != nullptr;
}

FSoundFXSettings* USoundFXDataAsset::GetSoundFXSettings(FGameplayTag Tag)
{
	return FXMap.Find(Tag);
}

USceneComponent* USoundFXDataAsset::ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params)
{
	if (!WorldContext || !WorldContext->GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("World Context Invalid!")));
		return nullptr;
	}
	
	if (FSoundFXSettings* FoundSound = GetSoundFXSettings(Params.FXTag))
	{
		if (!FoundSound->IsValid())
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *Params.FXTag.ToString()));
			return nullptr;
		}

		DEBUG_SIMPLE(
			LogSoundFX, 
			Log, 
			FColor::White, 
			FString::Printf(TEXT("Playing %s (%s)"), *Params.FXTag.ToString(), *UEnum::GetValueAsString(Params.PlayType)),
			Sf_GameplayTags::Effects::FX::SoundsFX::Name);

		UAudioComponent* AudioComp = nullptr;
		switch (Params.PlayType) {
			case EFXPlayType::FX_2D:
				AudioComp = UGameplayStatics::SpawnSound2D(
					WorldContext->GetWorld(),
					FoundSound->SoundFileType->GetSoundFile(),
					FoundSound->Volume,
					FoundSound->Pitch,
					FoundSound->StartTime,
					FoundSound->Concurrency);
				break;
			case EFXPlayType::FX_Location:
				AudioComp = UGameplayStatics::SpawnSoundAtLocation(
					WorldContext->GetWorld(),
					FoundSound->SoundFileType->GetSoundFile(),
					Params.Transform.GetLocation(),
					Params.Transform.GetRotation().Rotator(),
					FoundSound->Volume,
					FoundSound->Pitch,
					FoundSound->StartTime,
					FoundSound->Attenuation,
					FoundSound->Concurrency);
				break;
			case EFXPlayType::FX_Attached:
				AudioComp = UGameplayStatics::SpawnSoundAttached(
					FoundSound->SoundFileType->GetSoundFile(),
					Params.AttacheSceneComponent,
					Params.Bone,
					Params.Transform.GetLocation(),
					Params.Transform.GetRotation().Rotator(),
					EAttachLocation::KeepRelativeOffset,
					false,
					FoundSound->Volume,
					FoundSound->Pitch,
					FoundSound->StartTime,
					FoundSound->Attenuation,
					FoundSound->Concurrency);
				break;
		}

		if (!AudioComp)
		{
			return nullptr;
		}
		
		for (auto Processor : FoundSound->Processors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyFX(AudioComp);
		}

		return AudioComp;
	}
	return nullptr;
}

bool USoundFXDataAsset::CancelFX_Implementation(USceneComponent* FXComponent)
{
	if (UAudioComponent* AudioComponent = Cast<UAudioComponent>(FXComponent))
	{
		AudioComponent->Stop();
		return true;
	}
	return false;
}

float USoundFXDataAsset::GetFXDuration_Implementation(USceneComponent* FXComponent)
{
	if (UAudioComponent* AudioComponent = Cast<UAudioComponent>(FXComponent))
	{
		if (AudioComponent->Sound->IsLooping())
			return -1;
		return AudioComponent->Sound->Duration;
	}
	return 0;
}
