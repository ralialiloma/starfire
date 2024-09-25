// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimDataEditor.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Starfire/Animation/WeaponAnimationAssetType.h"
#include "Starfire/Animation/WeaponAnimationMontageType.h"
#include "Starfire/Animation/WeaponBlendSpaceType.h"
#include "StarfireEditor/Utility/EditorHelpers.h"

TSharedRef<IPropertyTypeCustomization> FWeaponAnimDataEditor::MakeInstance()
{
	return MakeShareable(new FWeaponAnimDataEditor);
}

void FWeaponAnimDataEditor::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	EditorHelpers::DisplayDefaultHeader(PropertyHandle, HeaderRow);
}

void FWeaponAnimDataEditor::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TArray<TSharedPtr<IPropertyHandle>> EditedProperties {};

	TSharedPtr<IPropertyHandle> AssetsHandle = PropertyHandle->GetChildHandle("AnimationAssets", false);
	EditedProperties.Add(AssetsHandle);
	if (AssetsHandle.IsValid())
	{
		uint32 ChildCount = 0;
		AssetsHandle->GetNumChildren(ChildCount);
		IDetailGroup& Group = ChildBuilder.AddGroup("Animation Assets", FText::FromString("Animation Assets"));

		TMap<EWeaponAnimationAssetType, UAnimSequenceBase*>* AnimationAssetsMap = nullptr;
		EditorHelpers::GetRawData(AssetsHandle, AnimationAssetsMap);

		for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
		{
			auto ChildHandle = AssetsHandle->GetChildHandle(ChildIndex);
			FText NameText = FText::FromString("Error");
			
			if (AnimationAssetsMap)
			{
				auto Value = GetKeyValueAtIndex(*AnimationAssetsMap, ChildIndex);
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

	TSharedPtr<IPropertyHandle> MontagesHandle = PropertyHandle->GetChildHandle("AnimationMontages", false);
	EditedProperties.Add(MontagesHandle);
	if (MontagesHandle.IsValid())
	{
		uint32 ChildCount = 0;
		MontagesHandle->GetNumChildren(ChildCount);
		IDetailGroup& Group = ChildBuilder.AddGroup("Animation Montages", FText::FromString("Animation Montages"));

		TMap<EWeaponAnimationMontageType, UAnimMontage*>* AnimationMontagesMap = nullptr;
		EditorHelpers::GetRawData(MontagesHandle, AnimationMontagesMap);

		for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
		{
			auto ChildHandle = MontagesHandle->GetChildHandle(ChildIndex);
			FText NameText = FText::FromString("Error");
			
			if (AnimationMontagesMap)
			{
				auto Value = GetKeyValueAtIndex(*AnimationMontagesMap, ChildIndex);
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

	TSharedPtr<IPropertyHandle> BlendspacesHandle = PropertyHandle->GetChildHandle("Blendspaces", false);
	EditedProperties.Add(BlendspacesHandle);
	if (BlendspacesHandle.IsValid())
	{
		uint32 ChildCount = 0;
		BlendspacesHandle->GetNumChildren(ChildCount);
		IDetailGroup& Group = ChildBuilder.AddGroup("Blendspaces", FText::FromString("Blendspaces"));

		TMap<EWeaponBlendSpaceType, UAnimMontage*>* BendspacesMap = nullptr;
		EditorHelpers::GetRawData(BlendspacesHandle, BendspacesMap);

		for (uint32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
		{
			auto ChildHandle = BlendspacesHandle->GetChildHandle(ChildIndex);
			FText NameText = FText::FromString("Error");
			
			if (BendspacesMap)
			{
				auto Value = GetKeyValueAtIndex(*BendspacesMap, ChildIndex);
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
	
	EditorHelpers::DisplayDefaultChildren(PropertyHandle, ChildBuilder, EditedProperties);
}
