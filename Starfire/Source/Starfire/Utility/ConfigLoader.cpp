
#include "ConfigLoader.h"

#if WITH_EDITOR
void FConfigLoader::LoadConfigFile(UObject* ObjectToLoad, FString Name, TArray<FName> AdditionalProperties)
{
	FString CustomConfigFilePath = GetConfigFilePath(Name);

	if (!ObjectToLoad || GConfig == nullptr)
		return;
		
	for (TFieldIterator<FProperty> PropIt(ObjectToLoad->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		if (!Property)
			continue;

		if (!Property->HasMetaData(TEXT("CustomConfig")) && AdditionalProperties.Contains(Property->GetFName()))
			continue;
		
		LoadProperty<UObject>(Property, ObjectToLoad, Name, CustomConfigFilePath);
	}
	
	UE_LOG(Sf_ConfigLoaderLog, Log, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
}

void FConfigLoader::SaveCustomConfig(UObject* ObjectToSave, FString Name, TArray<FName> AdditionalProperties)
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

		if (!Property->HasMetaData(TEXT("CustomConfig")) && !AdditionalProperties.Contains(Property->GetFName()))
			continue;

		bool bSuccessfulSave = false;
		SaveProperty<UObject>(Property, ObjectToSave, Name, CustomConfigFilePath, bSuccessfulSave);
		if (bSuccessfulSave)
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
#endif