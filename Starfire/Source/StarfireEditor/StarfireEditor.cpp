#include "StarfireEditor.h"

#include "Animation/HSMConfigEditor.h"
#include "Modules/ModuleManager.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Animation/WeaponAnimDataEditor.h"
#include "Player/Sf_CharacterEditor.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"

IMPLEMENT_MODULE(FStarfireEditorModule, StarfireEditor)

#define LOCTEXT_NAMESPACE "FStarfireEditorModule"

void FStarfireEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.RegisterCustomPropertyTypeLayout("WeaponAnimData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FWeaponAnimDataEditor::MakeInstance));
    PropertyEditorModule.RegisterCustomPropertyTypeLayout("HSMConfig", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FHSMConfigEditor::MakeInstance));
    PropertyEditorModule.RegisterCustomClassLayout(ASf_FP_Character::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&Sf_CharacterEditor::MakeInstance));
}

void FStarfireEditorModule::ShutdownModule()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.UnregisterCustomPropertyTypeLayout("WeaponAnimData");
    PropertyEditorModule.UnregisterCustomPropertyTypeLayout("HSMConfig");
    PropertyEditorModule.UnregisterCustomClassLayout(ASf_FP_Character::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE