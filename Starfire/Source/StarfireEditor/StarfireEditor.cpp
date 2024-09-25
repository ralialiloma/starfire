#include "StarfireEditor.h"

#include "Animation/HSMConfigEditor.h"
#include "Modules/ModuleManager.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Animation/WeaponAnimDataEditor.h"

IMPLEMENT_MODULE(FStarfireEditorModule, StarfireEditor)

#define LOCTEXT_NAMESPACE "FStarfireEditorModule"

void FStarfireEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.RegisterCustomPropertyTypeLayout("WeaponAnimData", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FWeaponAnimDataEditor::MakeInstance));
    PropertyEditorModule.RegisterCustomPropertyTypeLayout("HSMConfig", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FHSMConfigEditor::MakeInstance));
}

void FStarfireEditorModule::ShutdownModule()
{
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.UnregisterCustomPropertyTypeLayout("WeaponAnimData");
    PropertyEditorModule.UnregisterCustomPropertyTypeLayout("HSMConfig");
}

#undef LOCTEXT_NAMESPACE