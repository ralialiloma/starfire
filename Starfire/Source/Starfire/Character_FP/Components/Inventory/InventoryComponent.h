#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ResourceCraftingDefinition.h"
#include "InventoryComponent.generated.h"

class UResourceCraftingDefinition;

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All);

UCLASS(Blueprintable)
class STARFIRE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	int AddResource(FGameplayTag ItemTag, int AddQuantity);

	UFUNCTION(BlueprintCallable)
	bool HasQuantity(FGameplayTag ItemTag, int Quantity);

	UFUNCTION(BlueprintCallable)
	bool ConsumeResource(FGameplayTag ItemTag, int Quantity);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CraftItem(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int CraftItems(FGameplayTag ItemTag, int Quantity, bool CraftMax = true);

	UFUNCTION(BlueprintCallable)
	int GetItemMaxStack(FGameplayTag ItemTag);
	
protected:

	UPROPERTY(BlueprintReadOnly, SaveGame)
	TMap<FGameplayTag, int> ResourceMap;

	UPROPERTY(EditDefaultsOnly)
	UResourceCraftingDefinition* CraftingDefinitions;
};
