#pragma once
#include "ConfigLoader.generated.h"

USTRUCT()
struct FConfigLoader
{
	GENERATED_BODY()
	
	template <class T>
	static void LoadConfigFile(T* ObjectToLoad, FString Name);

	template <class T>
	static void SaveCustomConfig(T* ObjectToSave, FString Name);

	template <class T>
	static void LoadProperty(FProperty*, T* ObjectToLoad, FString Name,FString CustomConfigFilePath);

private:
	static FString GetSectionName(FString Name);
	static FString GetConfigFilePath(FString Name);
};


	template <typename T>
	void FConfigLoader::LoadConfigFile(T* ObjectToLoad, FString Name)
	{
		FString CustomConfigFilePath = GetConfigFilePath(Name);

		if (!ObjectToLoad || GConfig == nullptr)
			return;
		
		for (TFieldIterator<FProperty> PropIt(T::StaticClass()); PropIt; ++PropIt)
		{
			//if (PropIt->GetOwnerClass() != T::StaticClass())
			//	continue;
			
			FProperty* Property = *PropIt;
			LoadProperty(Property, ObjectToLoad, Name, CustomConfigFilePath);
		}

		
		UE_LOG(LogTemp, Warning, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
	}

	template <class T>
	void FConfigLoader::LoadProperty(FProperty* Property, T* ObjectToLoad, FString Name, FString CustomConfigFilePath)
	{
		if (!Property->HasMetaData(TEXT("CustomConfig")))
			return;

		FString PropertyName = Property->GetName();
		FString SectionName = GetSectionName(Name);

		//Float
		FFloatProperty* FloatProperty = static_cast<FFloatProperty*>(Property);
		if (FloatProperty)
		{
			float Value;
			if (GConfig->GetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath))
			{
				float* ValuePtr = FloatProperty->ContainerPtrToValuePtr<float>(ObjectToLoad);
				*ValuePtr = Value;
			}
		}
	}

	template <typename T>
	void FConfigLoader::SaveCustomConfig(T* ObjectToSave,FString Name)
	{
		if (!ObjectToSave || GConfig == nullptr)
			return;

		FString CustomConfigFilePath = GetConfigFilePath(Name);
		FString SectionName = GetSectionName(Name);
		
		//GConfig->EmptySection(*SectionName, CustomConfigFilePath);
		
		int PropsWithMetaTag = 0;
		for (TFieldIterator<FProperty> PropIt(T::StaticClass(),EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;

			if (!Property->HasMetaData(TEXT("CustomConfig")))
				continue;

			if (PropIt->GetOwnerClass() != T::StaticClass())
				continue;

			FString PropertyName = Property->GetName();

			//GConfig->RemoveKey(*SectionName, *PropertyName, CustomConfigFilePath);
			//GConfig->EmptySection(*SectionName, CustomConfigFilePath);

			FFloatProperty* FloatProperty = static_cast<FFloatProperty*>(Property);
			
			if (FloatProperty)
			{
				float Value = *FloatProperty->ContainerPtrToValuePtr<float>(ObjectToSave);
				GConfig->SetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath);
				PropsWithMetaTag++;
				continue;
			}
			FIntProperty* IntProperty = static_cast<FIntProperty*>(Property);
			if (IntProperty)
			{
				int32 Value = *IntProperty->ContainerPtrToValuePtr<int32>(ObjectToSave);
				GConfig->SetInt(*SectionName, *PropertyName, Value, CustomConfigFilePath);
				PropsWithMetaTag++;
				continue;
			}
		}

		if (PropsWithMetaTag<=0)
		{
			return;
		}
		
		GConfig->Flush(false, CustomConfigFilePath);
		UE_LOG(LogTemp, Warning, TEXT("Saved custom config to %s"), *CustomConfigFilePath);
	}


