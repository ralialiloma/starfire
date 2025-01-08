// Fill out your copyright notice in the Description page of Project Settings.


#include "FXSubsystem.h"
#include "DebugFunctionLibrary.h"
#include "FXDataAssetBase.h"
#include "FXSystemSettings.h"
#include "MessageFXPairingDataAsset.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogFXSubsystem);

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
	
	DEBUG_SIMPLE(
	LogFXSubsystem, 
	Log, 
	FColor::White, 
	FString::Printf(TEXT("Requested 2D %s"), *FXTag.ToString()), 
	Sf_GameplayTags::Effects::Name);


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

FFXHandle UFXSubsystem::PlayFXAt(FGameplayTag FXTag, FTransform Transform)
{
	if (!AllReferencesValid())
		return FFXHandle();

	DEBUG_SIMPLE(
		LogFXSubsystem, 
		Log, 
		FColor::White, 
		FString::Printf(TEXT("Requested %s at %s"), *FXTag.ToString(), *Transform.ToString()), 
		Sf_GameplayTags::Effects::Name);

	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : MessageFXPairings->GetMappedFX(FXTag))
		{
			FXDataAsset->ExecuteFX(this, FFXParams(FX, Transform));
		}
	}

	//TODO
	return FFXHandle::GenerateNewHandle();
}

FFXHandle UFXSubsystem::PlayFXOn(FGameplayTag FXTag, USceneComponent* Component, FName Bone, FTransform Offset)
{
	if (!AllReferencesValid())
		return FFXHandle();

	if (!Component)
		return FFXHandle();

	DEBUG_SIMPLE(
	LogFXSubsystem, 
	Log, 
	FColor::White, 
	FString::Printf(TEXT("Requested %s on %s (%s with offset %s)"), *FXTag.ToString(), *Component->GetName(), *Bone.ToString(), *Offset.ToString()), 
	Sf_GameplayTags::Effects::Name);

	for (auto FXDataAsset : FXDataAssets)
	{
		for (auto FX : MessageFXPairings->GetMappedFX(FXTag))
		{
			FXDataAsset->ExecuteFX(this, FFXParams(FX, Component, Bone, Offset));
		}
	}

	const FVector Start = Component->GetComponentTransform().GetLocation() + Offset.GetLocation();
	const FVector End = Component->GetComponentTransform().GetLocation() + Offset.GetLocation() +
		(Component->GetComponentTransform().GetRotation() * Offset.GetRotation()).GetForwardVector() * 50;
	UDebugFunctionLibrary::DebugDrawArrow(this, Sf_GameplayTags::Effects::Name, Start, End, 12, FColor::Silver, 1);
	
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
		MessageFXPairings = Settings->MessageFXPairings.LoadSynchronous();

		if (Settings->FXDataAssets.Num() > 0)
		{			
			for (auto FXDataAsset : Settings->FXDataAssets)
			{
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
