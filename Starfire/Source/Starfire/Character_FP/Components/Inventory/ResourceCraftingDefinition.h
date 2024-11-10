#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ResourceCraftingDefinition.generated.h"

USTRUCT(BlueprintType)
struct FItemQuantityDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gameplay.Resource"))
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;
	
};

USTRUCT(BlueprintType)
struct FCraftingData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemQuantityDefinition CraftedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemQuantityDefinition> RequiredResources;

	bool IsValid() const;
	
};

UCLASS(Blueprintable)
class STARFIRE_API UResourceCraftingDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	FCraftingData GetCraftingData(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable)
	int GetMaxStack(FGameplayTag ItemTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCraftingData> CraftingData {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FItemQuantityDefinition> MaxStackData {};
};
