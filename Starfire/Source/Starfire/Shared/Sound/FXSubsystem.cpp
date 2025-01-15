// Fill out your copyright notice in the Description page of Project Settings.


#include "FXSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "FXDataAssetBase.h"
#include "FXSystemSettings.h"
#include "MessageFXPairingDataAsset.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogFXSubsystem);

UFXSubsystem* UFXSubsystem::Get()
{
	return GEngine->GetEngineSubsystem<UFXSubsystem>();
}

FFXHandle UFXSubsystem::PlayFX(const UObject* WorldContext, FGameplayTag FXTag)
{
	if (!AllReferencesValid())
		return FFXHandle();
	
	UWorld* World = WorldContext != nullptr ? WorldContext->GetWorld() : GetWorld();
	
	DEBUG_SIMPLE(
	LogFXSubsystem, 
	Log, 
	FColor::White, 
	FString::Printf(TEXT("Requested 2D %s"), *FXTag.ToString()), 
	Sf_GameplayTags::Effects::Name);

	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);
	
	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : FXTags)
		{
			FXDataAsset->ExecuteFX(World, FX);
		}
	}

	//TODO
	return FFXHandle::GenerateNewHandle();
}

FFXHandle UFXSubsystem::PlayFXAt(const UObject* WorldContext, FGameplayTag FXTag, FTransform Transform)
{
	if (!AllReferencesValid())
		return FFXHandle();

	UWorld* World = WorldContext != nullptr ? WorldContext->GetWorld() : GetWorld();

	DEBUG_SIMPLE(
		LogFXSubsystem, 
		Log, 
		FColor::White, 
		FString::Printf(TEXT("Requested %s at %s"), *FXTag.ToString(), *Transform.ToString()), 
		Sf_GameplayTags::Effects::Name);

	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);

	for (auto FXDataAsset : FXDataAssets)
	{
		
		for (auto FX : FXTags)
		{
			FXDataAsset->ExecuteFX(World, FFXParams(FX, Transform));
		}
	}

	//TODO
	return FFXHandle::GenerateNewHandle();
}

FFXHandle UFXSubsystem::PlayFXOn(const UObject* WorldContext, FGameplayTag FXTag, USceneComponent* Component, FName Bone, FTransform Offset)
{
	if (!AllReferencesValid())
		return FFXHandle();

	if (!Component)
		return FFXHandle();

	UWorld* World = WorldContext != nullptr ? WorldContext->GetWorld() : GetWorld();

	DEBUG_SIMPLE(
	LogFXSubsystem, 
	Log, 
	FColor::White, 
	FString::Printf(TEXT("Requested %s on %s (%s with offset %s)"), *FXTag.ToString(), *Component->GetName(), *Bone.ToString(), *Offset.ToString()), 
	Sf_GameplayTags::Effects::Name);

	FGameplayTagContainer FXTags = GetFXByMessageTag(FXTag);

	for (auto FXDataAsset : FXDataAssets)
	{
		for (FGameplayTag FX : FXTags)
		{
			FXDataAsset->ExecuteFX(World, FFXParams(FX, Component, Bone, Offset));
		}
	}

	const FVector Start = Component->GetComponentTransform().GetLocation() + Offset.GetLocation();
	const FVector End = Component->GetComponentTransform().GetLocation() + Offset.GetLocation() +
		(Component->GetComponentTransform().GetRotation() * Offset.GetRotation()).GetForwardVector() * 50;
	UDebugFunctionLibrary::DebugDrawArrow(World, Sf_GameplayTags::Effects::Name, Start, End, 12, FColor::Silver, 1);
	
	//TODO
	return FFXHandle::GenerateNewHandle();
}

bool UFXSubsystem::CancelFX(FFXHandle Handle)
{
	return false;
}

bool UFXSubsystem::AllReferencesValid() const
{
	if (MessageFXPairings.Num() <= 0)
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

UWorld* UFXSubsystem::GetWorld() const
{
	if (GEngine)
	{
		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			if (WorldContext.World())
			{
				return WorldContext.World();
			}
		}
	}

	return nullptr; 
}

FGameplayTagContainer UFXSubsystem::GetFXByMessageTag(FGameplayTag MessageTag) const
{
	FGameplayTagContainer FXTags {};
	for (UMessageFXPairingDataAsset* MessageFXPairing : MessageFXPairings)
	{
		if (IsValid(MessageFXPairing))
			FXTags.AppendTags(MessageFXPairing->GetMappedFX(MessageTag));
	}
	
	return FXTags;
}

void UFXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UFXSystemSettings* Settings = GetDefault<UFXSystemSettings>();
	if (Settings)
	{
		for (auto MessageFXPairing : Settings->MessageFXPairings)
		{
			MessageFXPairings.Add(MessageFXPairing.LoadSynchronous());
		}

		if (Settings->FXDataAssets.Num() > 0)
		{			
			for (auto FXDataAsset : Settings->FXDataAssets)
			{
				UFXDataAssetBase* Asset = FXDataAsset.LoadSynchronous();
				if (IsValid(Asset))
					FXDataAssets.Add(Asset);
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
