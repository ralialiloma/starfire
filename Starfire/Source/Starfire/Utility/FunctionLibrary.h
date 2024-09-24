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
class STARFIRE_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable,meta  = (ExpandEnumAsExecs = "SignalType"))
	static EInputSignalType ConvertToInputSignalType(EInputSignalType SignalType);

	template<typename EnumType>
	static TArray<EnumType> GetAllEnumValues(bool ExcludeZero = false);

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


