#pragma once
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorHelpers.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Starfire/Utility/CollectionHelper.h"

struct EnumMapEditorHelpers
{
public:
	template<typename TEnum, typename TOther>
	static void DisplayFixedEnumMap(TSharedPtr<IPropertyHandle> MapHandle, IDetailChildrenBuilder& ChildBuilder);
};

template <typename TEnum, typename TOther>
void EnumMapEditorHelpers::DisplayFixedEnumMap(TSharedPtr<IPropertyHandle> MapHandle, IDetailChildrenBuilder& ChildBuilder)
{	
	if (MapHandle.IsValid())
	{
		uint32 ChildCount = 0;
		MapHandle->GetNumChildren(ChildCount);

		FString PropertyName = MapHandle->GetProperty()->GetName();
		IDetailGroup& Group = ChildBuilder.AddGroup(FName(PropertyName), FText::FromString(PropertyName));

		TMap<TEnum, TOther>* EquipmentFlagsMap = nullptr;
		EditorHelpers::GetRawData(MapHandle, EquipmentFlagsMap);

		for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
		{
			auto ChildHandle = MapHandle->GetChildHandle(ChildIndex);
			FText NameText = FText::FromString("Error");
			
			if (EquipmentFlagsMap)
			{
				auto Value = FCollectionHelper::GetKeyValueAtIndex(*EquipmentFlagsMap, ChildIndex);
				NameText = UEnum::GetDisplayValueAsText(Value.Key);
			}

			Group.AddWidgetRow()
			.NameContent()
			[
				SNew(STextBlock)
				.Text(NameText)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			.ValueContent()
			[
				ChildHandle->CreatePropertyValueWidget()
			];
		}
	}
}
