// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "FXDataAssetBase.generated.h"

struct FGameplayTag;

UENUM(BlueprintType)
enum class EFXPlayType : uint8
{
	FX_2D,
	FX_Location,
	FX_Attached
};

USTRUCT(BlueprintType)
struct FFXParams
{
	GENERATED_BODY()

	FFXParams() {  }
	FFXParams(const FGameplayTag InFXTag)
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_2D;
	}
	FFXParams(const FGameplayTag InFXTag, const FVector& InLocation)
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_Location;
		Location = InLocation;
	}
	FFXParams(const FGameplayTag InFXTag, USceneComponent* InAttachComponent, FName InBone = NAME_None, const FVector& InOffset = FVector::ZeroVector)
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_Attached;
		Location = InOffset;
		AttacheSceneComponent = InAttachComponent;
		Bone = InBone;
	}

	
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Effects.FX"))
	FGameplayTag FXTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFXPlayType PlayType = EFXPlayType::FX_2D;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* AttacheSceneComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Bone = NAME_None;
};
/**
 * 
 */
UCLASS(Abstract)
class STARFIRE_API UFXDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (WorldContext = "WorldContext"))
	void ExecuteFX(UObject* WorldContext, FFXParams Params);
};
