// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputSignalType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealType.h"
#include "UObject/UnrealTypePrivate.h"
#include "FunctionLibrary.generated.h"

#pragma region Enums

UENUM(BlueprintType)
enum  ESuccessState
{
	Success,
	Failed,
};
#pragma endregion

UCLASS()
class PROJECT_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable,meta  = (ExpandEnumAsExecs = "SignalType"))
	static EInputSignalType ConvertToInputSignalType(EInputSignalType SignalType);

	template<typename EnumType>
	static TArray<EnumType> GetAllEnumValues(bool ExcludeZero = false);

	template <class T>
	static void LoadConfigFile(T* ObjectToLoad, FString Name);

	template <class T>
	static void SaveCustomConfig(T* ObjectToSave, FString checkName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static  int ConvertEnumToInteger(uint8 Byte);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass"))
	static UObject* GetAsType(TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert);

	UFUNCTION(BlueprintCallable, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass", ExpandEnumAsExecs = "Success"))
	static UObject* Cast(TEnumAsByte<ESuccessState>& Success, TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass"))
	static TSubclassOf<UObject> MakeClass(TSubclassOf<UObject> OutputClass)
	{
		return OutputClass;
	}
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Debug")
	static FColor BoolToColor(bool bValue);

	UFUNCTION(BlueprintCallable, Category="Collision", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", DisplayName="Box Overlap Actors"))
	static bool BetterBoxOverlapActors(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& OutActors);


	UFUNCTION(BlueprintCallable, Category="Collision", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", DisplayName="Box Overlap Components"))
	static bool BetterBoxOverlapComponents(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot, FVector Extent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class UPrimitiveComponent*>& OutComponents);

};

template <typename EnumType>
TArray<EnumType> UFunctionLibrary::GetAllEnumValues(bool ExcludeZero)
{
	static_assert(TIsEnum<EnumType>::Value, "GetAllEnumValues can only be used with enum types!");
	
	const UEnum* EnumInfo = StaticEnum<EnumType>();
	if (!EnumInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid enum type!"));
		return {};
	}
	
	int32 MaxEnumValue = EnumInfo->GetMaxEnumValue();
	
	TArray<EnumType> EnumValues;
	EnumValues.Reserve(MaxEnumValue);
	
	for (int32 i = 0; i < MaxEnumValue; i++)
	{
		if (i==0 && ExcludeZero)
			continue;

		EnumValues.Add(static_cast<EnumType>(i));
	}

	return EnumValues;
}

template <typename T>
void UFunctionLibrary::LoadConfigFile(T* ObjectToLoad, FString Name)
{
	FString CustomConfigFilePath = FPaths::ProjectConfigDir() + Name + TEXT(".ini");

	if (!ObjectToLoad || GConfig == nullptr)
		return;
	
	for (TFieldIterator<UProperty> PropIt(ObjectToLoad->GetClass()); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;
		
		if (Property->HasMetaData(TEXT("CustomConfig")))
		{
			FString PropertyName = Property->GetName();
			FString SectionName = TEXT("/Script/YourProject.") + Name;
			const UFloatProperty* FloatProperty = static_cast<UFloatProperty*>(Property); 
			if (IsValid(FloatProperty))
			{
				float Value;
				if (GConfig->GetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath))
				{
					float* ValuePtr = FloatProperty->ContainerPtrToValuePtr<float>(ObjectToLoad);
					*ValuePtr = Value;
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Loaded custom config from %s"), *CustomConfigFilePath);
	
}

template <typename T>
void UFunctionLibrary::SaveCustomConfig(T* ObjectToSave,FString Name)
{
	if (!ObjectToSave || GConfig == nullptr)
		return;

	FString CustomConfigFilePath = FPaths::ProjectConfigDir() +Name +TEXT(".ini");

	int ProppsWithMetaTag = 0;
	for (TFieldIterator<FProperty> PropIt(ObjectToSave-> GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		UE_LOG(LogTemp, Log, TEXT("test"))

		if (!Property->HasMetaData(TEXT("CustomConfig")))
			continue;

		FString PropertyName = Property->GetName();
		FString SectionName = TEXT("/Script/YourProject.") + Name;

		FFloatProperty* FloatProperty = static_cast<FFloatProperty*>(Property);
		
		if (FloatProperty)
		{
			float Value = *FloatProperty->ContainerPtrToValuePtr<float>(ObjectToSave);
			GConfig->SetFloat(*SectionName, *PropertyName, Value, CustomConfigFilePath);
			UE_LOG(LogTemp, Log, TEXT("Found FloatProp"))
			ProppsWithMetaTag++;
			continue;
		}
		FIntProperty* IntProperty = static_cast<FIntProperty*>(Property);
		if (IntProperty)
		{
			int32 Value = *IntProperty->ContainerPtrToValuePtr<int32>(ObjectToSave);
			GConfig->SetInt(*SectionName, *PropertyName, Value, CustomConfigFilePath);
			ProppsWithMetaTag++;
			continue;
		}
	}

	if (ProppsWithMetaTag<=0)
	{
		return;
	}
	
	GConfig->Flush(false, CustomConfigFilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved custom config to %s"), *CustomConfigFilePath);
}
