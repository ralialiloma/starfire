// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "FXSubsystem.generated.h"

class UFXDataAssetBase;
class UVisualFXDataAsset;
class UMessageFXPairingDataAsset;
class USoundFXDataAsset;
class USoundBase;

USTRUCT(BlueprintType)
struct FFXHandle
{
	GENERATED_BODY()

public:
	
	FFXHandle()
		: Handle(NAME_None)
	{
	}
	
	explicit FFXHandle(const FName& InHandle)
		: Handle(InHandle)
	{
	}

	static FFXHandle GenerateNewHandle()
	{
		return FFXHandle(FName(*FString::Printf(TEXT("FXHandle_%d"), FMath::Rand())));
	}
	
	FName GetHandle() const
	{
		return Handle;
	}
	
	void Invalidate()
	{
		Handle = NAME_None;
	}

	bool IsValid() const
	{
		return Handle != NAME_None;
	}

	bool operator==(const FFXHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FFXHandle& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FFXHandle& FXHandle)
	{
		return GetTypeHash(FXHandle.Handle);
	}

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName Handle;
};

UCLASS()
class STARFIRE_API UFXSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	static UFXSubsystem* Get(const UObject* WorldContext);
	
	UFUNCTION(BlueprintCallable, Category="FXSystem")
	FFXHandle PlayFX(UPARAM(meta=(Categories="Effects.Messages")) FGameplayTag FXTag);

	UFUNCTION(BlueprintCallable, Category="FXSystem")
	FFXHandle PlayFXAt(UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXTag, FTransform Transform);

	UFUNCTION(BlueprintCallable, Category="FXSystem", meta = (AdvancedDisplay = "2"))
	FFXHandle PlayFXOn(UPARAM(meta = (Categories = "Effects.Messages")) FGameplayTag FXTag, USceneComponent* Component, FName Bone = NAME_None, FTransform Offset = FTransform());

	UFUNCTION(BlueprintCallable, Category="FXSystem")
	bool CancelFX(FFXHandle Handle);

protected:
	
	FGameplayTagContainer GetFXByMessageTag(FGameplayTag MessageTag) const;
	bool AllReferencesValid() const;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FXSystem")
	TArray<UFXDataAssetBase*> FXDataAssets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FXSystem")
	UMessageFXPairingDataAsset* MessageFXPairings;
};
