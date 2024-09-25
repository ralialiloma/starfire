// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class STARFIREEDITOR_API FWeaponAnimDataEditor : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	
	template<typename KeyType, typename ValueType>
	TPair<KeyType, ValueType> GetKeyValueAtIndex(const TMap<KeyType, ValueType>& Map, int32 Index)
	{
		TPair<KeyType,ValueType> OutPair = TPair<KeyType, ValueType>();
		// Check if the index is within the bounds of the map
		if (Index < 0 || Index >= Map.Num())
		{
			return TPair<KeyType, ValueType>(); // Invalid index
		}

		int32 CurrentIndex = 0;

		// Iterate over the map to find the key-value pair at the given index
		for (auto It = Map.CreateConstIterator(); It; ++It)
		{
			if (CurrentIndex == Index)
			{
				// Set the key and value when we reach the desired index
				OutPair.Key = It.Key();
				OutPair.Value = It.Value();
				return OutPair; // Successfully found the key-value pair
			}

			++CurrentIndex;
		}

		return TPair<KeyType, ValueType>(); // Should not reach here if the index is valid
	}
};
