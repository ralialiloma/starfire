// Fill out your copyright notice in the Description page of Project Settings.

#include "Starfire.h"
#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "Character_FP/Sf_FP_Character.h"
#include "Editor/Player/Sf_CharacterEditor.h"
#endif

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Starfire, "Starfire" );

void FStarfire::StartupModule()
{
	IModuleInterface::StartupModule();

#if WITH_EDITOR
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(ASf_FP_Character::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&Sf_CharacterEditor::MakeInstance));
#endif

}

void FStarfire::ShutdownModule()
{
	IModuleInterface::ShutdownModule();

#if WITH_EDITOR
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout(ASf_FP_Character::StaticClass()->GetFName());
#endif
	
	
}
