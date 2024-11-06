#include "InventoryComponent.h"

#include "ResourceCraftingDefinition.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

void UInventoryComponent::AddResource(FGameplayTag ItemTag, int Quantity)
{
	if (!ItemTag.IsValid())
		return;
	
	Quantity = FMath::Max(0, Quantity);
	int& ItemQuantity = ResourceMap.FindOrAdd(ItemTag);
	ItemQuantity += Quantity;

	FString DebugString("Added " + FString::FromInt(Quantity) + " of Item: " + ItemTag.ToString());
	SF_SIMPLE_DEBUG(LogInventoryComponent, Log, FColor::White, *DebugString, Inventory::Name);
}

bool UInventoryComponent::HasQuantity(FGameplayTag ItemTag, int Quantity)
{
	if (!ItemTag.IsValid())
		return false;
	
	if (int* ItemQuantity = ResourceMap.Find(ItemTag))
	{
		if (Quantity <= *ItemQuantity)
			return true;
	}
	return false;
}

bool UInventoryComponent::ConsumeResource(FGameplayTag ItemTag, int Quantity)
{
	if (!HasQuantity(ItemTag, Quantity))
		return false;
	
	Quantity = FMath::Max(0, Quantity);
	int& ItemQuantity = ResourceMap.FindOrAdd(ItemTag);
	ItemQuantity -= Quantity;

	FString DebugString("Consumed " + FString::FromInt(Quantity) + " of Item: " + ItemTag.ToString());
	SF_SIMPLE_DEBUG(LogInventoryComponent, Log, FColor::White, *DebugString, Inventory::Name);
	return true;
}

bool UInventoryComponent::CraftItem_Implementation(FGameplayTag ItemTag)
{
	if (!CraftingDefinitions)
	{
		return false;
	}

	FCraftingData CraftData = CraftingDefinitions->GetCraftingData(ItemTag);
	if (!CraftData.IsValid())
	{
		return false;
	}

	for (auto Resource : CraftData.RequiredResources)
	{
		if (!HasQuantity(Resource.ItemTag, Resource.Quantity))
			return false;
	}

	for (auto Resource : CraftData.RequiredResources)
	{
		ConsumeResource(Resource.ItemTag, Resource.Quantity);
	}

	AddResource(CraftData.CraftedItem.ItemTag, CraftData.CraftedItem.Quantity);
	return true;
}

int UInventoryComponent::CraftItems_Implementation(FGameplayTag ItemTag, int Quantity, bool CraftMax)
{
	if (!CraftingDefinitions)
	{
		return 0;
	}
	
	FCraftingData CraftData = CraftingDefinitions->GetCraftingData(ItemTag);
	if (!CraftData.IsValid())
	{
		return false;
	}

	// If it should craft as many as possible
	if (CraftMax)
	{
		for (int i = 0; i < Quantity; ++i)
		{
			if (!CraftItem(ItemTag))
				return i;
		}
		return Quantity;
	}

	// If it should only craft if it can craft all at once
	for (auto Resource : CraftData.RequiredResources)
	{
		if (!HasQuantity(Resource.ItemTag, Resource.Quantity * Quantity))
			return 0;
	}

	for (auto Resource : CraftData.RequiredResources)
	{
		ConsumeResource(Resource.ItemTag, Resource.Quantity * Quantity);
	}

	AddResource(CraftData.CraftedItem.ItemTag, CraftData.CraftedItem.Quantity * Quantity);
	return Quantity;
}
