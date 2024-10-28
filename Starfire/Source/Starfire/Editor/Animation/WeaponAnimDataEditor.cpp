// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimDataEditor.h"

#if WITH_EDITOR
#include "Starfire/Editor/Utility/EditorHelpers.h"
#include "Starfire/Editor/Utility/EnumMapEditorHelpers.h"

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

	/*TSharedPtr<IPropertyHandle> AssetsHandle = PropertyHandle->GetChildHandle("AnimationAssets", false);
	EditedProperties.Add(AssetsHandle);
	EnumMapEditorHelpers::DisplayFixedEnumMap<EWeaponAnimationAssetType_FP, UAnimSequenceBase*>(AssetsHandle, ChildBuilder);

	TSharedPtr<IPropertyHandle> MontagesHandle = PropertyHandle->GetChildHandle("AnimationMontages", false);
	EditedProperties.Add(MontagesHandle);
	EnumMapEditorHelpers::DisplayFixedEnumMap<EWeaponAnimationMontageType_FP, UAnimMontage*>(MontagesHandle, ChildBuilder);

	TSharedPtr<IPropertyHandle> BlendspacesHandle = PropertyHandle->GetChildHandle("Blendspaces", false);
	EditedProperties.Add(BlendspacesHandle);
	EnumMapEditorHelpers::DisplayFixedEnumMap<EWeaponBlendSpaceType_FP, UBlendSpace*>(BlendspacesHandle, ChildBuilder);*/

	EditorHelpers::DisplayDefaultChildren(PropertyHandle, ChildBuilder, EditedProperties);
}

#endif
