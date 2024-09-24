#include "ConfigLoader.h"

FString FConfigLoader::GetSectionName(FString Name)
{
	return  TEXT("/Script/YourProject.") + Name ;
}

FString FConfigLoader::GetConfigFilePath(FString Name)
{
	return FPaths::ProjectConfigDir() + Name + TEXT(".ini");
}
