﻿#include "InventoryComponent.h"

#include "ResourceCraftingDefinition.h"
#include "DebugFunctionLibrary.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/ActionLogger/ActionLogger.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);


void UInventoryComponent::ClearResource(const FGameplayTag ItemTag)
{
	if (!ItemTag.IsValid())
		return;

	ConsumeResource(ItemTag, GetQuantity(ItemTag));
}

void UInventoryComponent::ClearAllResources()
{
	TArray<FGameplayTag> AllResources;
	AllResources.Empty();
	ResourceMap.GetKeys(AllResources);
	for (const FGameplayTag& ResourceTag : AllResources)
	{
		ClearResource(ResourceTag);
	}
}

int UInventoryComponent::AddResource(FGameplayTag ItemTag, int AddQuantity)
{
	if (!ItemTag.IsValid())
		return AddQuantity;
	
	AddQuantity = FMath::Max(0, AddQuantity);
	int ReturnQuantity = 0;
	int& ItemQuantity = ResourceMap.FindOrAdd(ItemTag);

	const int NewItemQuantity = ItemQuantity + AddQuantity;
	const int MaxItemStack = GetItemMaxStack(ItemTag);
	if (MaxItemStack <= 0 || NewItemQuantity <= MaxItemStack)
	{
		ItemQuantity = NewItemQuantity;
	}
	else
	{
		ItemQuantity = MaxItemStack;
		ReturnQuantity = NewItemQuantity - MaxItemStack;
	}

	OnResourceAddedInternal(ItemTag);
	OnResourceAdded.Broadcast(ItemTag,ItemQuantity);
	UpdateAvailableCraftables();
	
	FString DebugString("Added " + FString::FromInt(AddQuantity - ReturnQuantity) + " of Item: " + ItemTag.ToString());
	DEBUG_SIMPLE(LogInventoryComponent, Warning, FColor::White, *DebugString, Sf_GameplayTags::Debug::Inventory::Name);

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
	DEBUG_SIMPLE(LogInventoryComponent, Warning, FColor::White, *DebugString, Sf_GameplayTags::Debug::Inventory::Name);

	OnResourceRemoved.Broadcast(ItemTag,ItemQuantity);
	UpdateAvailableCraftables();
	
	return true;
}

bool UInventoryComponent::CanCraftItem_Implementation(FGameplayTag ItemTag)
{
	const FCraftingData CraftData = CraftingDefinitions->GetCraftingData(ItemTag);
	if (!CraftData.IsValid())
		return false;

	if (!CanCraftItem(CraftData))
		return false;

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

bool UInventoryComponent::IsFull(FGameplayTag ItemTag) const
{
	return GetQuantity(ItemTag) >= GetItemMaxStack(ItemTag);
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
	if (!Resource.IsValid() || !CraftingItem.IsValid())
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

bool UInventoryComponent::CanCraftItem(FCraftingData CraftingData) const
{
	for (FItemQuantityDefinition Resource : CraftingData.RequiredResources)
	{
		if (!HasQuantity(Resource.ItemTag, Resource.Quantity))
			return false;
	}

	return true;
}

void UInventoryComponent::UpdateAvailableCraftables()
{
	if (IsValid(CraftingDefinitions))
	{
		for (const FCraftingData& CraftingData : CraftingDefinitions->GetAllCraftingData())
		{
			FGameplayTag ItemTag = CraftingData.CraftedItem.ItemTag;
			const bool bHasSpaceForItem = GetQuantity(ItemTag)<GetItemMaxStack(ItemTag);
			if(CanCraftItem(CraftingData) && CraftingData.bAutoCraft && bHasSpaceForItem)
			{
				CraftItem(ItemTag);
			}
		}
	}
}

void UInventoryComponent::OnResourceAddedInternal_Implementation(FGameplayTag Item)
{
	UActionLoggerSubSystem* ActionLogger = UActionLoggerSubSystem::Get(GetWorld());
	if (Item.MatchesTagExact(Sf_GameplayTags::Gameplay::Resource::Enemy))
	{
		ActionLogger->ReportAction(Sf_GameplayTags::Gameplay::ActionLogger::Resources::PowerCore::Collect);
	}
	else if (Item.MatchesTagExact(Sf_GameplayTags::Gameplay::Resource::Environment))
	{
		ActionLogger->ReportAction(Sf_GameplayTags::Gameplay::ActionLogger::Resources::Crystal::Collect);
	}
	else if (Item.MatchesTagExact(Sf_GameplayTags::Gameplay::Resource::ObjectiveItem))
	{
		ActionLogger->ReportAction(Sf_GameplayTags::Gameplay::ActionLogger::FP::Equipment::Grenade::Craft);
	}
}

bool UInventoryComponent::CraftItem_Implementation(const FGameplayTag ItemTag)
{
	if (!CraftingDefinitions)
	{
		return false;
	}

	if (!CanCraftItem(ItemTag))
	{
		return false; 
	}
	
	FCraftingData CraftData = CraftingDefinitions->GetCraftingData(ItemTag);
	for (auto Resource : CraftData.RequiredResources)
	{
		ConsumeResource(Resource.ItemTag, Resource.Quantity);
	}
	
	AddResource(CraftData.CraftedItem.ItemTag, CraftData.CraftedItem.Quantity);

	UFXSubsystem::Get()->PlayFX(this, CraftData.FXCraft);
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
