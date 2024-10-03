#include "ConfigLoader.h"

void FConfigLoader::LoadConfigFile(UObject* ObjectToLoad, FString Name)
{
	FString CustomConfigFilePath = GetConfigFilePath(Name);

	if (!ObjectToLoad || GConfig == nullptr)
		return;
		
	for (TFieldIterator<FProperty> PropIt(ObjectToLoad->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		LoadProperty<UObject>(Property, ObjectToLoad, Name, CustomConfigFilePath);
	}
	
	UE_LOG(Sf_ConfigLoaderLog, Log, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
}

void FConfigLoader::SaveCustomConfig(UObject* ObjectToSave, FString Name)
{
	if (!ObjectToSave || GConfig == nullptr)
	{
		return;
	}

	FString CustomConfigFilePath = GetConfigFilePath(Name);
	FString SectionName = GetSectionName(Name);
	
	int SavedProperties = 0;
	for (TFieldIterator<FProperty> PropIt(ObjectToSave->GetClass(),EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		
		if (PropIt->GetOwnerClass() != ObjectToSave->GetClass())
			continue;

		bool SuccesFullSave = false;
		SaveProperty<UObject>(Property,ObjectToSave,Name,CustomConfigFilePath,SuccesFullSave);
		if (SuccesFullSave)
			SavedProperties++;
	}

	if (SavedProperties<=0)
	{
		return;
	}
	
	GConfig->Flush(false, CustomConfigFilePath);
	UE_LOG(Sf_ConfigLoaderLog, Log, TEXT("Saved custom config to %s"), *CustomConfigFilePath);
}

FString FConfigLoader::GetSectionName(FString Name)
{
	return  TEXT("/Script/YourProject.") + Name ;
}

FString FConfigLoader::GetConfigFilePath(FString Name)
{
	auto Path = FPaths::Combine(FPaths::ProjectConfigDir(), Name + TEXT(".ini"));
	FPaths::MakeStandardFilename(Path);
	return FConfigCacheIni::NormalizeConfigIniPath(Path);
}