// Fill out your copyright notice in the Description page of Project Settings.


#include "FXSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "FXSystemSettings.h"
#include "MessageFXPairingDataAsset.h"
#include "Visual/VisualFXDataAsset.h"

UFXSubsystem* UFXSubsystem::Get(const UObject* WorldContext)
{
	if (!WorldContext || !WorldContext->GetWorld())
		return nullptr;

	return WorldContext->GetWorld()->GetSubsystem<UFXSubsystem>();
}

FFXHandle UFXSubsystem::PlayFX(FGameplayTag FXTag)
{
	if (!AllReferencesValid())
		return FFXHandle();

	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : MessageFXPairings->GetMappedFX(FXTag))
		{
			FXDataAsset->ExecuteFX(this, FX);
		}
	}

	//TODO
	return FFXHandle::GenerateNewHandle();
}

FFXHandle UFXSubsystem::PlayFXAt(FGameplayTag FXTag, FVector Location)
{
	if (!AllReferencesValid())
		return FFXHandle();

	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : MessageFXPairings->GetMappedFX(FXTag))
		{
			FXDataAsset->ExecuteFX(this, FFXParams(FX, Location));
		}
	}

	//TODO
	return FFXHandle::GenerateNewHandle();
}

FFXHandle UFXSubsystem::PlayFXOn(FGameplayTag FXTag, USceneComponent* Component, FName Bone, FVector Offset)
{
	if (!AllReferencesValid())
		return FFXHandle();

	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : MessageFXPairings->GetMappedFX(FXTag))
		{
			FXDataAsset->ExecuteFX(this, FFXParams(FX, Component, Bone, Offset));
		}
	}
	
	//TODO
	return FFXHandle::GenerateNewHandle();
}

bool UFXSubsystem::CancelFX(FFXHandle Handle)
{
	return false;
}

bool UFXSubsystem::AllReferencesValid() const
{
	if (!MessageFXPairings)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No MessageFXPairs set on FXSubsystem.")));
		return false;
	}

	if (!GetWorld())
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No Valid World.")));
		return false;
	}

	if (FXDataAssets.Num() <= 0)
	{
		UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No FX Data Tables Assigned!")));
		return false;
	}
	
	return true;
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
	if (Settings)
	{
		if (Settings->MessageFXPairings.IsValid())
			MessageFXPairings = Settings->MessageFXPairings.LoadSynchronous();

		if (Settings->FXDataAssets.Num() > 0)
		{			
			for (auto FXDataAsset : Settings->FXDataAssets)
			{
				if (FXDataAsset.IsValid())
					FXDataAssets.Add(FXDataAsset.LoadSynchronous());
			}
		}
		else
		{
			UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("No FX Data Tables Assigned!")));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No SoundDataAsset assigned in Project Settings > Project > Sound System"));
	}
}
