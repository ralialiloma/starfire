#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ResourceCraftingDefinition.h"
#include "InventoryComponent.generated.h"

class UResourceCraftingDefinition;

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Display, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRessourceChange, FGameplayTag, Ressource, int, RemainingQuantity);

UCLASS(Blueprintable)
class STARFIRE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	int AddResource(FGameplayTag ItemTag, int AddQuantity);

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	bool HasQuantity(FGameplayTag ItemTag, int Quantity) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	int GetQuantity(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	bool ConsumeResource(FGameplayTag ItemTag, int Quantity);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Categories = "Gameplay.Resource"))
	bool CraftItem(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Categories = "Gameplay.Resource"))
	bool CanCraftItem(FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Categories = "Gameplay.Resource"))
	int CraftItems(FGameplayTag ItemTag, int Quantity, bool CraftMax = true);

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	int GetItemMaxStack(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	bool IsFull(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	void GetItemCraftingRequirements(FGameplayTag CraftingItem, TArray<FItemQuantityDefinition>& RequiredItems) const;

	UFUNCTION(BlueprintCallable, meta = (Categories = "Gameplay.Resource"))
	int GetItemCraftingRequirementsOfItem(FGameplayTag CraftingItem,FGameplayTag Resource) const;

protected:
	bool CanCraftItem(FCraftingData CraftingData) const;
	void UpdateAvailableCraftables();

public:

	UPROPERTY(BlueprintAssignable)
	FOnRessourceChange OnResourceAdded;

	UPROPERTY(BlueprintAssignable)
	FOnRessourceChange OnResourceRemoved;
	
protected:
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TMap<FGameplayTag, int> ResourceMap;

	UPROPERTY(EditDefaultsOnly)
	UResourceCraftingDefinition* CraftingDefinitions;
};
