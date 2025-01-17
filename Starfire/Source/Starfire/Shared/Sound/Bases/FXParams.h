#pragma once
#include "GameplayTagContainer.h"

#include "FXParams.generated.h"

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

public:
	
	FFXParams() {  }
	FFXParams(const FGameplayTag InFXTag)
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_2D;
	}
	FFXParams(const FGameplayTag InFXTag, const FTransform& InTransform)
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_Location;
		Transform = InTransform;
	}
	FFXParams(const FGameplayTag InFXTag, USceneComponent* InAttachComponent, FName InBone = NAME_None, const FTransform& InOffset = FTransform())
	{
		FXTag = InFXTag;
		PlayType = EFXPlayType::FX_Attached;
		Transform = InOffset;
		AttacheSceneComponent = InAttachComponent;
		Bone = InBone;
	}

	
	UPROPERTY(BlueprintReadWrite, meta = (Categories = "Effects.FX"))
	FGameplayTag FXTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFXPlayType PlayType = EFXPlayType::FX_2D;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Transform = FTransform();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* AttacheSceneComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Bone = NAME_None;
};