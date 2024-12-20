// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Templates/TypeHash.h"
#include "MessageFXPairingDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMessageFXPairings
{
	friend uint32 GetTypeHash(const FMessageFXPairings& Arg)
	{
		return GetTypeHash(Arg.MessageFX);
	}

	friend bool operator==(const FMessageFXPairings& Lhs, const FMessageFXPairings& RHS)
	{
		return Lhs.MessageFX == RHS.MessageFX;
	}

	friend bool operator!=(const FMessageFXPairings& Lhs, const FMessageFXPairings& RHS)
	{
		return !(Lhs == RHS);
	}

	GENERATED_BODY()

public:
	FMessageFXPairings() {  }
	FMessageFXPairings(FGameplayTag Tag)
	{
		MessageFX = Tag;
	}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Effects.Messages"))
	FGameplayTag MessageFX;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Effects.FX"))
	FGameplayTagContainer Effects;
};

UCLASS()
class STARFIRE_API UMessageFXPairingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	FGameplayTagContainer GetMappedFX(FGameplayTag Tag);

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSet<FMessageFXPairings> MappedFXs;
	
};