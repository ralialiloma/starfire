// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundFXDataAsset.h"

#include "DebugFunctionLibrary.h"
#include "SoundFXProcessor.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogSoundFX);

FSoundFXSettings* USoundFXDataAsset::GetSoundFXSettings(FGameplayTag Tag)
{
	return FXMap.Find(Tag);
}

void USoundFXDataAsset::ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params)
{
	if (!WorldContext || !WorldContext->GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("World Context Invalid!")));
		return;
	}
	
	if (FSoundFXSettings* FoundSound = GetSoundFXSettings(Params.FXTag))
	{
		if (!FoundSound->IsValid())
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found sound data is invalid for GameplayTag: %s"), *Params.FXTag.ToString()));
			return;
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
					FoundSound->SoundFile,
					FoundSound->Volume,
					FoundSound->Pitch,
					FoundSound->StartTime,
					FoundSound->Concurrency);
				break;
			case EFXPlayType::FX_Location:
				AudioComp = UGameplayStatics::SpawnSoundAtLocation(
					WorldContext->GetWorld(),
					FoundSound->SoundFile,
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
					FoundSound->SoundFile,
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
			return;
		}
		
		for (auto Processor : FoundSound->Processors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyFX(AudioComp);
		}
	}
}