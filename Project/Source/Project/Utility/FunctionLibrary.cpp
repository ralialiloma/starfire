// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"

EInputSignalType UFunctionLibrary::ConvertToInputSignalType(EInputSignalType SignalType)
{
	return  SignalType;
}

int UFunctionLibrary::ConvertEnumToInteger(uint8 Byte)
{
	return FMath::Pow(2,static_cast<float>(Byte));
}

UObject* UFunctionLibrary::GetAsType(TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert)
{
	{
		if (!ObjectToCovert)
		{
			return nullptr;
		}
		if (ObjectToCovert->GetClass()->IsChildOf(OutputClass))
		{
			return ObjectToCovert;
		}
		return nullptr;
	}
}

UObject* UFunctionLibrary::Cast(TEnumAsByte<ESuccessState>& Success, TSubclassOf<UObject> OutputClass,
	UObject* ObjectToCovert)
{
	{
		if (!ObjectToCovert)
		{
			Success = ESuccessState::Failed;
			return nullptr;
		}
		if (ObjectToCovert->GetClass()->IsChildOf(OutputClass))
		{
			Success = ESuccessState::Success;
			return ObjectToCovert;
		}
		Success = ESuccessState::Failed;
		return nullptr;
	}
}

FColor UFunctionLibrary::BoolToColor(bool bValue)
{
	return bValue?FColor::Green:FColor::Red;
}

bool UFunctionLibrary::BetterBoxOverlapActors(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot,
                                              FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter,
                                              const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
{
	OutActors.Empty();

	TArray<UPrimitiveComponent*> OverlapComponents;
	const bool bOverlapped = BetterBoxOverlapComponents(WorldContextObject, BoxPos, BoxRot, BoxExtent, ObjectTypes, NULL, ActorsToIgnore, OverlapComponents);
	if (bOverlapped)
	{
		UKismetSystemLibrary::GetActorListFromComponentList(OverlapComponents, ActorClassFilter, OutActors);
	}

	return (OutActors.Num() > 0);
}

bool UFunctionLibrary::BetterBoxOverlapComponents(const UObject* WorldContextObject, const FVector BoxPos,
                                                  const FRotator BoxRot, FVector BoxExtent, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
                                                  UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents)
{
	OutComponents.Empty();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(BoxOverlapComponents), false);
	Params.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr)
	{
		World->OverlapMultiByObjectType(Overlaps, BoxPos, BoxRot.Quaternion(), ObjectParams, FCollisionShape::MakeBox(BoxExtent), Params);
	}

	for (int32 OverlapIdx=0; OverlapIdx<Overlaps.Num(); ++OverlapIdx)
	{
		FOverlapResult const& O = Overlaps[OverlapIdx];
		if (O.Component.IsValid())
		{ 
			if ( !ComponentClassFilter || O.Component.Get()->IsA(ComponentClassFilter) )
			{
				OutComponents.Add(O.Component.Get());
			}
		}
	}

	return (OutComponents.Num() > 0);
}
