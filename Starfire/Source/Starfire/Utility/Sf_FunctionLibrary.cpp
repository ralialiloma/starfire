// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_FunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"


EInputSignalType USf_FunctionLibrary::ConvertToInputSignalType(EInputSignalType SignalType)
{
	return  SignalType;
}


ASf_FP_Character* USf_FunctionLibrary::GetSfPlayerpawn(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid WorldContextObject passed to GetSfPlayerpawn."));
		return nullptr;
	}

	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player pawn found."));
		return nullptr;
	}

	ASf_FP_Character* SfCharacter = dynamic_cast<ASf_FP_Character*>(Pawn);
	if (!SfCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player pawn is not of type ASf_FP_Character."));
	}

	return SfCharacter;
}

FVector USf_FunctionLibrary::GetPlayerLocation(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid WorldContextObject passed to GetPlayerLocation."));
		return FVector::Zero();
	}

	if (!IsValid(WorldContextObject->GetWorld()))
		return FVector::Zero();

	if (!IsValid(WorldContextObject->GetWorld()->GetGameInstance()))
		return FVector::Zero();

	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player pawn found."));
		return FVector::Zero();
	}

	return Pawn->GetActorLocation();
}

FString USf_FunctionLibrary::FormatStringWithNewlines(const FString& InputString)
{
	return InputString.Replace(TEXT(";"), TEXT("\n"));
}

int USf_FunctionLibrary::ConvertEnumToInteger(uint8 Byte)
{
	return FMath::Pow(2,static_cast<float>(Byte));
}

void USf_FunctionLibrary::SetBit(bool bSet, int32& BitField, int32 BitPosition)
{
	if (bSet)
		BitField |= (1 << BitPosition);
	else
		BitField &= ~(1 << BitPosition);
}

bool USf_FunctionLibrary::CheckBitFlag(int32 BitField, int32 BitPosition)
{
	return (BitField & (1 << BitPosition)) != 0;
}

UObject* USf_FunctionLibrary::GetAsType(TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert)
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

UObject* USf_FunctionLibrary::Cast(TEnumAsByte<ESuccessState>& Success, TSubclassOf<UObject> OutputClass,
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

FColor USf_FunctionLibrary::BoolToColor(bool bValue)
{
	return bValue?FColor::Green:FColor::Red;
}

bool USf_FunctionLibrary::BetterBoxOverlapActors(const UObject* WorldContextObject, const FVector BoxPos, const FRotator BoxRot,
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

bool USf_FunctionLibrary::BetterBoxOverlapComponents(const UObject* WorldContextObject, const FVector BoxPos,
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

