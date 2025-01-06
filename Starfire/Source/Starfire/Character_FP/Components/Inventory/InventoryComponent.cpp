#include "InventoryComponent.h"

#include "ResourceCraftingDefinition.h"
#include "DebugFunctionLibrary.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

int UInventoryComponent::AddResource(FGameplayTag ItemTag, int AddQuantity)
{
	if (!ItemTag.IsValid())
		return AddQuantity;
	
	AddQuantity = FMath::Max(0, AddQuantity);
	int ReturnQuantity = 0;
	int& ItemQuantity = ResourceMap.FindOrAdd(ItemTag);
	
	int NewItemQuantity = ItemQuantity + AddQuantity;
	int MaxItemStack = GetItemMaxStack(ItemTag);
	if (MaxItemStack <= 0 || NewItemQuantity < MaxItemStack)
	{
		ItemQuantity = NewItemQuantity;
	}
	else
	{
		ItemQuantity = MaxItemStack;
		ReturnQuantity = NewItemQuantity - MaxItemStack;
	}
		

	FString DebugString("Added " + FString::FromInt(AddQuantity - ReturnQuantity) + " of Item: " + ItemTag.ToString());
	DEBUG_SIMPLE(LogInventoryComponent, Log, FColor::White, *DebugString, Sf_GameplayTags::Debug::Inventory::Name);

	return ReturnQuantity;
}

bool UInventoryComponent::HasQuantity(FGameplayTag ItemTag, int Quantity) const
{
	return GetQuantity(ItemTag) >= Quantity;
}

int UInventoryComponent::GetQuantity(FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
		return 0;
	
	if (const int* ItemQuantity = ResourceMap.Find(ItemTag))
	{
		return *ItemQuantity;
	}
	return 0;
}

bool UInventoryComponent::ConsumeResource(FGameplayTag ItemTag, int Quantity)
{
	if (!HasQuantity(ItemTag, Quantity))
		return false;
	
	Quantity = FMath::Max(0, Quantity);
	int& ItemQuantity = ResourceMap.FindOrAdd(ItemTag);
	ItemQuantity -= Quantity;

	FString DebugString("Consumed " + FString::FromInt(Quantity) + " of Item: " + ItemTag.ToString());
	DEBUG_SIMPLE(LogInventoryComponent, Log, FColor::White, *DebugString, Sf_GameplayTags::Debug::Inventory::Name);
	return true;
}

int UInventoryComponent::GetItemMaxStack(FGameplayTag ItemTag) const
{
	if (!CraftingDefinitions)
	{
		return -1;
	}

	return CraftingDefinitions->GetMaxStack(ItemTag);
}

void UInventoryComponent::GetItemCraftingRequirements(FGameplayTag CraftingItem, TArray<FItemQuantityDefinition>& RequiredItems) const
{
	RequiredItems = TArray<FItemQuantityDefinition>();
	
	if (!CraftingDefinitions)
	{
		return;
	}
	
	FCraftingData CraftData = CraftingDefinitions->GetCraftingData(CraftingItem);
	if (!CraftData.IsValid())
	{
		return;
	}

	RequiredItems = CraftData.RequiredResources;
}

int UInventoryComponent::GetItemCraftingRequirementsOfItem(FGameplayTag CraftingItem, FGameplayTag Resource) const
{
	if (!Resource.IsValid())
		return 0;
	
	TArray<FItemQuantityDefinition> CraftingData {};
	GetItemCraftingRequirements(CraftingItem, CraftingData);

	for (auto Data : CraftingData)
	{
		if (Data.ItemTag == Resource)
			return Data.Quantity;
	}

	return 0;
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
