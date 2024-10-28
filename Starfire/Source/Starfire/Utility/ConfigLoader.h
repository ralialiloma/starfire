#pragma once
#include "ConfigLoader.generated.h"
DECLARE_LOG_CATEGORY_CLASS(Sf_ConfigLoaderLog, Log, All);

USTRUCT()
struct FConfigLoader
{
	GENERATED_BODY()

#if WITH_EDITOR
	template <class T>
	static void LoadConfigFile(T* ObjectToLoad, FString Name, TArray<FName> AdditionalProperties = {});
	static void LoadConfigFile(UObject* ObjectToLoad, FString Name, TArray<FName> AdditionalProperties = {});

	template <class T>
	static void SaveCustomConfig(T* ObjectToSave, FString Name, TArray<FName> AdditionalProperties = {});
	static void SaveCustomConfig(UObject* ObjectToSave, FString Name, TArray<FName> AdditionalProperties = {});
	
	template <class T>
	static  void SaveProperty(FProperty* Property, T* ObjectToSave, FString Name, FString CustomConfigFilePath,
	                 bool& SuccessfulSave);

	template <class T>
	static void LoadProperty(FProperty*, T* ObjectToLoad, FString Name,FString CustomConfigFilePath);

private:
	static FString GetSectionName(FString Name);
	static FString GetConfigFilePath(FString Name);
#endif
};

#if WITH_EDITOR
	template <typename T>
	void FConfigLoader::LoadConfigFile(T* ObjectToLoad, FString Name, TArray<FName> AdditionalProperties)
	{
		FString CustomConfigFilePath = GetConfigFilePath(Name);

		if (!ObjectToLoad || GConfig == nullptr)
			return;
		
		for (TFieldIterator<FProperty> PropIt(T::StaticClass()); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;

			if (!Property)
				continue;

			if (!Property->HasMetaData(TEXT("CustomConfig")) && AdditionalProperties.Contains(Property->GetFName()))
				continue;
			
			LoadProperty<T>(Property, ObjectToLoad, Name, CustomConfigFilePath);
		}
		
		UE_LOG(Sf_ConfigLoaderLog, Log, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
	}

	template <class T>
	void FConfigLoader::LoadProperty(FProperty* Property, T* ObjectToLoad, FString Name, FString CustomConfigFilePath)
	{
		if (!Property)
			return;

		if (!ObjectToLoad)
			return;

		FString PropertyName = Property->GetName();
		FString SectionName = GetSectionName(Name);

		//Float
		FFloatProperty* FloatProperty = static_cast<FFloatProperty*>(Property);
		
		if (FloatProperty)
		{
			float Value =0;
			if (GConfig->GetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath))
			{
				float* ValuePtr = FloatProperty->ContainerPtrToValuePtr<float>(ObjectToLoad);
				*ValuePtr = Value;
			}
		}
		

		//Int
		if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			int32 Value;
			if (GConfig->GetInt(*SectionName, *PropertyName, Value, CustomConfigFilePath))
			{
				int32* ValuePtr = IntProperty->ContainerPtrToValuePtr<int32>(ObjectToLoad);
				*ValuePtr = Value;
				return;
			}
		}
		
		//Bool
		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool Value;
			if (GConfig->GetBool(*SectionName, *PropertyName, Value, CustomConfigFilePath))
			{
				bool* ValuePtr = BoolProperty->ContainerPtrToValuePtr<bool>(ObjectToLoad);
				*ValuePtr = Value;
				return;
			}
		}
		
		//FString
		if (FStrProperty* StringProperty = CastField<FStrProperty>(Property))
		{
			FString Value;
			if (GConfig->GetString(*SectionName, *PropertyName, Value, CustomConfigFilePath))
			{
				FString* ValuePtr = StringProperty->ContainerPtrToValuePtr<FString>(ObjectToLoad);
				*ValuePtr = Value;
				return;
			}
		}
		
		
		//FVector
		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			if (StructProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FString Value;
				if (GConfig->GetString(*SectionName, *PropertyName, Value, CustomConfigFilePath))
				{
					FVector ParsedVector;
					if (ParsedVector.InitFromString(Value))
					{
						FVector* ValuePtr = StructProperty->ContainerPtrToValuePtr<FVector>(ObjectToLoad);
						*ValuePtr = ParsedVector;
						return;
					}
				}
			}
		}

		if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			FString AssetPath;
			FString NormalizedConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(*CustomConfigFilePath);
			
			if (GConfig->GetString(*SectionName, *PropertyName, AssetPath, NormalizedConfigFilePath))
			{
				if (!AssetPath.IsEmpty())
				{
					UObject* LoadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
					
					if (LoadedObject)
					{
						ObjectProperty->SetObjectPropertyValue_InContainer(ObjectToLoad, LoadedObject);
						return;
					}
				}
				else
				{
					UE_LOG(Sf_ConfigLoaderLog, Warning, TEXT("Asset path is empty for property: %s in section: %s"), *PropertyName, *SectionName);
				}
			}
			else
			{
				UE_LOG(Sf_ConfigLoaderLog, Warning, TEXT("Could not find property: %s in section: %s in config file: %s"), *PropertyName, *SectionName, *NormalizedConfigFilePath);
			}
		}
		
	}

	template <typename T>
	void FConfigLoader::SaveCustomConfig(T* ObjectToSave,FString Name, TArray<FName> AdditionalProperties)
	{
		if (!ObjectToSave || GConfig == nullptr)
		{
			return;
		}

		FString CustomConfigFilePath = GetConfigFilePath(Name);
		FString SectionName = GetSectionName(Name);
		
		int SavedProperties = 0;
		for (TFieldIterator<FProperty> PropIt(T::StaticClass(),EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			
			if (PropIt->GetOwnerClass() != T::StaticClass())
				continue;

			if (!Property->HasMetaData(TEXT("CustomConfig")) && !AdditionalProperties.Contains(Property->GetFName()))
				continue;

			bool bSuccessfulSave = false;
			SaveProperty<T>(Property,ObjectToSave,Name,CustomConfigFilePath,bSuccessfulSave);
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

	template <class T>
	void FConfigLoader::SaveProperty(FProperty* Property, T* ObjectToSave, FString Name, FString CustomConfigFilePath, bool& SuccessfulSave)
	{

		FString PropertyName = Property->GetName();
		FString SectionName = GetSectionName(Name);

		//UObject
		if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			UObject* ObjectValue = ObjectProperty->GetObjectPropertyValue_InContainer(ObjectToSave);
			if (ObjectValue)
			{
				FString AssetPath = ObjectValue->GetPathName();
				GConfig->SetString(*SectionName, *PropertyName, *AssetPath, CustomConfigFilePath);
				SuccessfulSave = true;
				return;
			}
		}

		// Float
		FFloatProperty* FloatProperty = static_cast<FFloatProperty*>(Property);
		if (FloatProperty)
		{
			float Value = *FloatProperty->ContainerPtrToValuePtr<float>(ObjectToSave);
			GConfig->SetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath);
			UE_LOG(Sf_ConfigLoaderLog, Log, TEXT("SavedFloat"));
			SuccessfulSave = true;
			return;
		}

		// Int
		FIntProperty* IntProperty = static_cast<FIntProperty*>(Property);
		if (IntProperty)
		{
			int32 Value = *IntProperty->ContainerPtrToValuePtr<int32>(ObjectToSave);
			GConfig->SetInt(*SectionName, *PropertyName, Value, CustomConfigFilePath);
			SuccessfulSave = true;
			return;
		}
		
		// Bool
		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			bool Value = *BoolProperty->ContainerPtrToValuePtr<bool>(ObjectToSave);
			GConfig->SetBool(*SectionName, *PropertyName, Value, CustomConfigFilePath);
			SuccessfulSave = true;
			return;
		}
		
		// FString
		if (FStrProperty* StringProperty = CastField<FStrProperty>(Property))
		{
			FString Value = *StringProperty->ContainerPtrToValuePtr<FString>(ObjectToSave);
			GConfig->SetString(*SectionName, *PropertyName, *Value, CustomConfigFilePath);
			SuccessfulSave = true;
			return;
		}
		
		//FVector
		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			if (StructProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector Value = *StructProperty->ContainerPtrToValuePtr<FVector>(ObjectToSave);
				GConfig->SetString(*SectionName, *PropertyName, *Value.ToString(), CustomConfigFilePath);
				SuccessfulSave = true;
				return;
			}
		}

		
		SuccessfulSave = false;
	}
#endif