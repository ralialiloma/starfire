#include "Sf_CharacterEditor.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"

TSharedRef<IDetailCustomization> Sf_CharacterEditor::MakeInstance()
{
	return MakeShareable(new Sf_CharacterEditor);
}

void Sf_CharacterEditor::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("CustomConfig", FText::GetEmpty(), ECategoryPriority::Important);
	ASf_FP_Character* Character = nullptr;
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (auto Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Character = Cast<ASf_FP_Character>(Object);
			if (Character)
				break;
		}
	}
	
	check(Character);

	Category.AddCustomRow(FText::FromString("Config"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Save Config"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	[
		SNew(SButton)
		.Text(FText::FromString("Save"))
		.ToolTipText(FText::FromString("Saves the set values to the custom config."))
		.HAlign(HAlign_Center)
		.OnClicked_UObject(Character, &ASf_FP_Character::OnSaveButtonClicked)
	];

	Category.AddCustomRow(FText::FromString("Config"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Load Config"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	[
		SNew(SButton)
		.Text(FText::FromString("Load"))
		.ToolTipText(FText::FromString("Loads saved values from config into the actor."))
		.HAlign(HAlign_Center)
		.OnClicked_UObject(Character, &ASf_FP_Character::OnLoadButtonClicked)
	];
	
}
