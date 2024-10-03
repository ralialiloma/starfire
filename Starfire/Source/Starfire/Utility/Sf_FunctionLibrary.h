// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputSignalType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealType.h"
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPath.h"
#include "Starfire/Character/Sf_Character.h"
#include "Engine/EngineTypes.h" 
#include "Sf_FunctionLibrary.generated.h"

#pragma region Enums

UENUM(BlueprintType)
enum  ESuccessState
{
	Success,
	Failed,
};
#pragma endregion

class ASf_Character;

DECLARE_LOG_CATEGORY_CLASS(Sf_FunctionLibrary_Log,Log, Log);
UCLASS()
class STARFIRE_API USf_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable,meta  = (ExpandEnumAsExecs = "SignalType"))
	static EInputSignalType ConvertToInputSignalType(EInputSignalType SignalType);

	template<typename EnumType>
	static TArray<EnumType> GetAllEnumValues(bool ExcludeZero = false);
	template<typename EnumType>
	static FString GetEnumAsString(EnumType EnumValue);
	template <class EnumType>
	static FString GetEnumAsString(int32 EnumValue);
	template <class EnumType>
	static FName GetEnumAsName(EnumType EnumValue);
	template <class EnumType>
	static FName GetEnumAsName(int32 EnumValue);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static ASf_Character* GetSfPlayerpawn(UWorld* WorldContext);

	template<typename RowType>
	static TArray<RowType> GetRowDataFromDT(const FSoftObjectPath& DTPath);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static  int ConvertEnumToInteger(uint8 Byte);

	UFUNCTION(BlueprintCallable, Category = "Bitwise Operation")
	static void SetBit(bool bSet, int32& BitField, int32 BitPosition);

	UFUNCTION(BlueprintCallable, Category = "Bitwise Operation")
	static bool CheckBitFlag(int32 BitField, int32 BitPosition);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass"))
	static UObject* GetAsType(TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert);

	UFUNCTION(BlueprintCallable, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass", ExpandEnumAsExecs = "Success"))
	static UObject* Cast(TEnumAsByte<ESuccessState>& Success, TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conversion", meta = (DeterminesOutputType = "OutputClass"))
	static TSubclassOf<UObject> MakeClass(TSubclassOf<UObject> OutputClass)
	{
		return OutputClass;
	}

	template <typename EnumType, typename AssetType>
	static void ValidateAndUpdateEnumMap(TMap<EnumType, AssetType>& AssetMap);
	template <typename EnumType, typename AssetType>
	static void ValidateAndUpdateEnumMap(TMap<TEnumAsByte<EnumType>, AssetType>& AssetMap,const TArray<TEnumAsByte<EnumType>>& ExcludedValues =TArray<TEnumAsByte<EnumType>>{});
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Debug")
	static FColor BoolToColor(bool bValue);

	UFUNCTION(BlueprintCallable, Category="Collision", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", DisplayName="Box Overlap Actors"))
	static bool BetterBoxOverlapActors(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& OutActors);


	UFUNCTION(BlueprintCallable, Category="Collision", meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore", DisplayName="Box Overlap Components"))
	static bool BetterBoxOverlapComponents(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot, FVector Extent, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class UPrimitiveComponent*>& OutComponents);
	

};

template <typename EnumType>
TArray<EnumType> USf_FunctionLibrary::GetAllEnumValues(bool ExcludeZero)
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

template <typename EnumType>
FString USf_FunctionLibrary::GetEnumAsString(EnumType EnumValue)
{
	static_assert(TIsEnum<EnumType>::Value, "GetEnumAsString can only be used with enum types!");
	return  GetEnumAsName<EnumType>(EnumValue).ToString();
}

template <typename EnumType>
FString USf_FunctionLibrary::GetEnumAsString(int32 EnumValue)
{
	static_assert(TIsEnum<EnumType>::Value, "GetEnumAsString can only be used with enum types!");
	return  GetEnumAsName<EnumType>(EnumValue).ToString();
}

template <typename EnumType>
FName USf_FunctionLibrary::GetEnumAsName(EnumType EnumValue)
{
	static_assert(TIsEnum<EnumType>::Value, "GetEnumAsName can only be used with enum types!");
	return  GetEnumAsName<EnumType>(static_cast<int32>(EnumValue));
}

template <typename EnumType>
FName USf_FunctionLibrary::GetEnumAsName(int32 EnumValue)
{
	static_assert(TIsEnum<EnumType>::Value, "GetEnumAsName can only be used with enum types!");
	const UEnum* EnumInfo = StaticEnum<EnumType>();
	FName EnumName = EnumInfo->GetNameByValue(EnumValue);
	return EnumName;
}

template <typename RowType>
TArray<RowType> USf_FunctionLibrary::GetRowDataFromDT(const FSoftObjectPath& DTPath)
{
	UObject* ResolvedUObject = DTPath.ResolveObject();
	UDataTable* DataTable = static_cast<UDataTable*>(ResolvedUObject);

	if (!DataTable)
	{
		UE_LOG(LogTemp,Warning,TEXT("Could not find DT for %s"),*RowType::StaticStruct()->GetName());
		return TArray<RowType>{};
	}

	TObjectPtr<UScriptStruct> RowStruct = DataTable->RowStruct;

	if (!IsValid(RowStruct))
	{
		UE_LOG(LogTemp,Warning,TEXT("Invalid Row Struct in DT"))
		return TArray<RowType>{};
	}

	if (RowStruct != RowType::StaticStruct())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Wrong DT Row Struct. Row Struct needs to be of type %s but is of type %s instead "),
			*RowType::StaticStruct()->GetName(),*DataTable->RowStruct.GetClass()->GetName());
		return TArray<RowType>{};
	}

	TArray<RowType> RowDatas{};
	const TArray<FName> AllRowNames = DataTable->GetRowNames();
	for (const FName RowName:AllRowNames)
	{
		const RowType* Row = DataTable->FindRow<RowType>(RowName,"");

		if (Row==nullptr)
		{
			UE_LOG(
			LogTemp,
			Warning,
			TEXT("Found Invalid Row"));
			continue;
		}

		RowDatas.Add(*Row);
	}
	return RowDatas;
}

template <typename EnumType, typename AssetType>
void USf_FunctionLibrary::ValidateAndUpdateEnumMap(TMap<EnumType, AssetType>& AssetMap)
{
	static_assert(TIsEnum<EnumType>::Value, "ValidateAndUpdateEnumMap can only be used with enum types!");

	TArray<EnumType> AssetTypes = USf_FunctionLibrary::GetAllEnumValues<EnumType>(true);

	// Import New Enums
	for (EnumType Type : AssetTypes)
	{
		if (!AssetMap.Contains(Type))
			AssetMap.Add(Type, nullptr);
	}

	// Remove Empty Enum Entries (None)
	AssetMap.Remove(EnumType(0));

	// Collect Invalid Enum Keys for Removal
	const UEnum* EnumInfo = StaticEnum<EnumType>();
	TArray<EnumType> KeysToRemove;

	for (const auto& Kvp : AssetMap)
	{
		if (!EnumInfo->IsValidEnumValue(static_cast<int64>(Kvp.Key)))
		{
			KeysToRemove.Add(Kvp.Key);
		}
	}

	// Remove invalid keys outside of the iteration
	for (const auto& Key : KeysToRemove)
	{
		AssetMap.Remove(Key);
	}
}

template <typename EnumType, typename AssetType>
void USf_FunctionLibrary::ValidateAndUpdateEnumMap(
	TMap<TEnumAsByte<EnumType>, AssetType>& AssetMap,
	const TArray<TEnumAsByte<EnumType>>& ExcludedValues)
{
	static_assert(TIsEnum<EnumType>::Value, "ValidateAndUpdateEnumMap can only be used with enum types!");

	// Get all enum values
	TArray<EnumType> AssetTypes = USf_FunctionLibrary::GetAllEnumValues<EnumType>(true);
	const UEnum* EnumInfo = StaticEnum<EnumType>();

	// Import New Enums, excluding specified values
	for (EnumType Type : AssetTypes)
	{
		TEnumAsByte<EnumType> EnumValue = TEnumAsByte<EnumType>(Type);
		if (!ExcludedValues.Contains(EnumValue) && !AssetMap.Contains(EnumValue))
		{
			AssetMap.Add(EnumValue, nullptr);
		}
	}

	// Remove Empty Enum Entries (None)
	AssetMap.Remove(TEnumAsByte<EnumType>(0));

	// Collect Invalid Enum Keys for Removal
	TArray<TEnumAsByte<EnumType>> KeysToRemove;

	for (const auto& Kvp : AssetMap)
	{
		if (!EnumInfo->IsValidEnumValue(Kvp.Key) || ExcludedValues.Contains(Kvp.Key))
		{
			KeysToRemove.Add(Kvp.Key);
		}
	}

	// Remove invalid keys outside of the iteration
	for (const auto& Key : KeysToRemove)
	{
		AssetMap.Remove(Key);
	}
}


