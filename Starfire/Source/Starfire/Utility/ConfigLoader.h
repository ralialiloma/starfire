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
	static  void SaveProperty(FProperty* Property, T* ObjectToSave, FString Name, FString CustomConfigFilePath,
	                 bool& SuccessfulSave);

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
			FProperty* Property = *PropIt;
			LoadProperty<T>(Property, ObjectToLoad, Name, CustomConfigFilePath);
		}

		
		UE_LOG(LogTemp, Warning, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
	}

	template <class T>
	void FConfigLoader::LoadProperty(FProperty* Property, T* ObjectToLoad, FString Name, FString CustomConfigFilePath)
	{
		if (!Property)
			return;

		if (!ObjectToLoad)
			return;
		
		if (!Property->HasMetaData(TEXT("CustomConfig")))
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
			if (GConfig->GetString(*SectionName, *PropertyName, AssetPath, CustomConfigFilePath))
			{
				UObject* LoadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
				if (LoadedObject)
				{
					ObjectProperty->SetObjectPropertyValue_InContainer(ObjectToLoad, LoadedObject);
					return;
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to load object from path: %s"), *AssetPath);
				}
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
		
		int SavedProperties = 0;
		for (TFieldIterator<FProperty> PropIt(T::StaticClass(),EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			
			if (PropIt->GetOwnerClass() != T::StaticClass())
				continue;

			bool SuccesFullSave = false;
			SaveProperty<T>(Property,ObjectToSave,Name,CustomConfigFilePath,SuccesFullSave);
			if (SuccesFullSave)
				SavedProperties++;
		}

		if (SavedProperties<=0)
		{
			return;
		}
		
		GConfig->Flush(false, CustomConfigFilePath);
		UE_LOG(LogTemp, Warning, TEXT("Saved custom config to %s"), *CustomConfigFilePath);
	}

	template <class T>
	void FConfigLoader::SaveProperty(FProperty* Property, T* ObjectToSave, FString Name, FString CustomConfigFilePath, bool& SuccessfulSave)
	{
		if (!Property->HasMetaData(TEXT("CustomConfig")))
			return;

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
			UE_LOG(LogTemp, Log, TEXT("SavedFloat"));
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


