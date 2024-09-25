#include "EditorHelpers.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

void EditorHelpers::DisplayChildError(IDetailChildrenBuilder& ChildBuilder, const FString& Text)
{
	ChildBuilder.AddCustomRow(FText::FromString("Error"))
				.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Error:"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
		.ColorAndOpacity(FColor::Red)
	]
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(Text))
		.Font(IDetailLayoutBuilder::GetDetailFont())
		.ColorAndOpacity(FColor::Red)
	];
}

void EditorHelpers::DisplayEmptyChildren(IDetailChildrenBuilder& ChildBuilder)
{
	ChildBuilder.AddCustomRow(FText::FromString("Error"))
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("No Children Found."))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

void EditorHelpers::DisplayCustomTextChild(IDetailChildrenBuilder& ChildBuilder, const FString& NameContent,
	const FString& ValueContent)
{
	ChildBuilder.AddCustomRow(FText::FromString("Error"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(NameContent))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(ValueContent))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

void EditorHelpers::DisplayDefaultHeader(const TSharedRef<IPropertyHandle>& PropertyHandle, FDetailWidgetRow& HeaderRow)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];

	HeaderRow.ValueContent()
	[
		PropertyHandle->CreatePropertyValueWidget()
	];
}

void EditorHelpers::DisplayDefaultChildren(const TSharedRef<IPropertyHandle>& PropertyHandle,
                                           IDetailChildrenBuilder& ChildBuilder, const TArray<TSharedPtr<IPropertyHandle>>& ExcludedHandles)
{
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		TSharedPtr<IPropertyHandle> ChildProperty = PropertyHandle->GetChildHandle(ChildIndex);
		
		bool bIsExcluded = ExcludedHandles.ContainsByPredicate([&](const TSharedPtr<IPropertyHandle>& ExcludedHandle)
		{
			return ExcludedHandle.IsValid() && ExcludedHandle->GetProperty() == ChildProperty->GetProperty();
		});

		if (bIsExcluded)
			continue;
		
		ChildBuilder.AddProperty(ChildProperty.ToSharedRef());
	}
}