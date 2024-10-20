#include "HSMConfigEditor.h"

#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "StarfireEditor/Utility/EditorHelpers.h"
#include "StarfireEditor/Utility/EnumMapEditorHelpers.h"

class UBaseState;

TSharedRef<IPropertyTypeCustomization> FHSMConfigEditor::MakeInstance()
{
	return MakeShareable(new FHSMConfigEditor);
}

void FHSMConfigEditor::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
                                       IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	EditorHelpers::DisplayDefaultHeader(PropertyHandle, HeaderRow);
}

void FHSMConfigEditor::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TArray<TSharedPtr<IPropertyHandle>> EditedProperties {};

	const TSharedPtr<IPropertyHandle> EquipmentFlagHandle = PropertyHandle->GetChildHandle("EquipmentFlagMap", false);
	EditedProperties.Add(EquipmentFlagHandle);
	EnumMapEditorHelpers::DisplayFixedEnumMap<TEnumAsByte<EEquipmentFlags>, TSubclassOf<UBaseState>>(EquipmentFlagHandle, ChildBuilder);
	
	EditorHelpers::DisplayDefaultChildren(PropertyHandle, ChildBuilder, EditedProperties);
}