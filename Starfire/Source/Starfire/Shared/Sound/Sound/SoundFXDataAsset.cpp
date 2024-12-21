// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundFXDataAsset.h"

#include "DebugFunctionLibrary.h"
#include "SoundFXProcessor.h"
#include "Kismet/GameplayStatics.h"

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
					Params.Location,
					FRotator::ZeroRotator, 
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
					Params.Location,
					FRotator::ZeroRotator,
					EAttachLocation::KeepRelativeOffset,
					false,
					FoundSound->Volume,
					FoundSound->Pitch,
					FoundSound->StartTime,
					FoundSound->Attenuation,
					FoundSound->Concurrency);
				break;
		}
		
		for (auto Processor : FoundSound->Processors)
		{
			if (!Processor)
				continue;
			
			Processor->ModifyFX(AudioComp);
		}
	}
}