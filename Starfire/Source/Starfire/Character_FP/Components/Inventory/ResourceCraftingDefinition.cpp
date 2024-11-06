#include "ResourceCraftingDefinition.h"

bool FCraftingData::IsValid() const
{
	return CraftedItem.ItemTag.IsValid() && CraftedItem.Quantity > 0;
}

FCraftingData UResourceCraftingDefinition::GetCraftingData(FGameplayTag ItemTag)
{
	FCraftingData* Data = CraftingData.FindByPredicate(
		[ItemTag](const FCraftingData& CraftingData){
		if (ItemTag.IsValid() && ItemTag.MatchesTag(CraftingData.CraftedItem.ItemTag))
			return true;
		return false;
	});

	if (!Data)
		return FCraftingData();

	return *Data;
}
