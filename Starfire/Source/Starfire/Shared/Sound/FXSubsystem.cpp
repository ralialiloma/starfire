// Fill out your copyright notice in the Description page of Project Settings.


#include "FXSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "FXSystemSettings.h"
#include "MessageFXPairingDataAsset.h"
#include "Bases/FXDataAssetBase.h"
#include "Bases/FXHandle.h"
#include "Bases/FXHandleObject.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Bases/FXParams.h"

DEFINE_LOG_CATEGORY(LogFXSubsystem);

UFXSubsystem* UFXSubsystem::Get()
{
	return GEngine->GetEngineSubsystem<UFXSubsystem>();
}

FFXHandle UFXSubsystem::PlayFX(const UObject* WorldContext, FGameplayTag FXMessageTag)
{
	if (!AllReferencesValid())
		return FFXHandle();
	
	DEBUG_SIMPLE(
	LogFXSubsystem, 
	Log, 
	FColor::White, 
	FString::Printf(TEXT("Requested 2D %s"), *FXMessageTag.ToString()), 
	Sf_GameplayTags::Effects::Name);

	return PlayFX_Internal(WorldContext, FXMessageTag, FGameplayTag());
}

FFXHandle UFXSubsystem::PlayFXAt(const UObject* WorldContext, FGameplayTag FXMessageTag, FTransform Transform)
{
	if (!AllReferencesValid())
		return FFXHandle();

	DEBUG_SIMPLE(
		LogFXSubsystem, 
		Log, 
		FColor::White, 
		FString::Printf(TEXT("Requested %s at %s"), *FXMessageTag.ToString(), *Transform.ToString()), 
		Sf_GameplayTags::Effects::Name);

	return PlayFX_Internal(WorldContext, FXMessageTag, FFXParams(FGameplayTag(), Transform));
}

FFXHandle UFXSubsystem::PlayFXOn(const UObject* WorldContext, FGameplayTag FXMessageTag, USceneComponent* Component, FName Bone, FTransform Offset)
{
	if (!AllReferencesValid())
		return FFXHandle();

	if (!Component)
		return FFXHandle();

	DEBUG_SIMPLE(
		LogFXSubsystem, 
		Log, 
		FColor::White, 
		FString::Printf(TEXT("Requested %s on %s (%s with offset %s)"), *FXMessageTag.ToString(), *Component->GetName(), *Bone.ToString(), *Offset.ToString()), 
		Sf_GameplayTags::Effects::Name);

	return PlayFX_Internal(WorldContext, FXMessageTag, FFXParams(FGameplayTag(), Component, Bone, Offset));
}

bool UFXSubsystem::CancelFX(FFXHandle& Handle)
{
	TWeakObjectPtr<UFXHandleObject>* FoundObject = FXDataHandles.Find(Handle);
	if (!FoundObject)
		return true;

	TWeakObjectPtr<UFXHandleObject> WeakObject = FoundObject->Get();
	if (WeakObject.IsValid())
	{
		WeakObject->Cancel();
	}

	FXDataHandles.Remove(Handle);
	Handle.Invalidate();
	return true;
}

FFXHandle UFXSubsystem::PlayFX_Internal(const UObject* WorldContext, FGameplayTag FXMessageTag, FFXParams Params)
{
	UWorld* World = WorldContext != nullptr ? WorldContext->GetWorld() : GetWorld();
	FGameplayTagContainer FXTags = GetFXByMessageTag(FXMessageTag);

	TObjectPtr<UFXHandleObject> FXHandleObject = NewObject<UFXHandleObject>();
	for (auto FXDataAsset : FXDataAssets)
	{
		auto& WrapperRef = FXHandleObject->FXAssetComponentPairings.Add(FXDataAsset);
		for (FGameplayTag FX : FXTags)
		{
			Params.FXTag = FX;
			if (USceneComponent* FXComponent = FXDataAsset->ExecuteFX(World, Params))
				WrapperRef.Components.Add(FXComponent);
		}
	}

	FFXHandle Handle {};
	if (FXHandleObject->CanBeCanceled())
	{
		Handle = FFXHandle::GenerateNewHandle();
		FXHandleObject->OnCompleteData(WorldContext->GetWorld(), Handle);
		FXDataHandles.Add(Handle, FXHandleObject);
	}

	return Handle;
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
