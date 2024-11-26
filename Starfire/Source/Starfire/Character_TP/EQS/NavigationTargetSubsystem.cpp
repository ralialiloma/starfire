

#include "NavigationTargetSubsystem.h"

#include "Sf_AiSettings.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


void UNavigationTargetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (SHOULD_DEBUG(TP::EQS::NavigationsTargets,EDebugType::Visual))
	{
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle,[this]()->void{DebugTick();},Tickrate,true);
	}

}

void UNavigationTargetSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UNavigationTargetSubsystem::DebugTick()
{
	for (const TTuple<AActor*,FVector>  Target: ActorToTarget)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Target.Value,150.f,6,FColor::Blue,Tickrate+0.01f,2);
	}
}


//Reserved Covers
void UNavigationTargetSubsystem::RegisterNavTarget(AActor* PursuingAgent,FVector LocationToRegister)
{
	if (!IsValid(PursuingAgent))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Trying to add invalid PursuingAgent");
		return;
	}

	if (ActorToTarget.Contains(PursuingAgent))
		return;

	FString DebugString =
		FString::Printf(TEXT("Registered %s at %s"),*PursuingAgent->GetName(),*UKismetStringLibrary::Conv_VectorToString(LocationToRegister));
	UDebugFunctionLibrary::Sf_PrintString(this,DebugString,Sf_GameplayTags::Debug::TP::EQS::NavigationsTargets);
	ActorToTarget.Add(PursuingAgent,LocationToRegister);

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//if (USf_AISettings::Get()->NavBlockerToSpawn)
		//Navblockers.Add(
		//	GetWorld()->SpawnActor(USf_AISettings::Get()->NavBlockerToSpawn,&LocationToRegister,&FRotator::ZeroRotator,ActorSpawnParameters)); 

	//ActiveNavTargets.Add(LocationToRegister);
}

void UNavigationTargetSubsystem::UnregisterNavTarget(AActor* PursuingAgent)
{;
	FVector* FoundLocationRef =  ActorToTarget.Find(PursuingAgent);
	if (FoundLocationRef == nullptr)
		return;

	FVector FoundLocation = *FoundLocationRef;
	
	TArray<AActor*> TempBlockers = Navblockers;
	for (AActor* Blocker: TempBlockers)
	{
		if (!IsValid(Blocker))
		{
			Navblockers.Remove(Blocker);
			continue;
		}

		if(FVector::Distance(Blocker->GetActorLocation(),FoundLocation)<150.f)
		{
			Navblockers.Remove(Blocker);
			Blocker->Destroy();
		}
			
	}

	ActorToTarget.Remove(PursuingAgent);
	
	//ActiveNavTargets.Remove(LocationToRegister);
}

void UNavigationTargetSubsystem::ClearReservedCovers()
{
	ActorToTarget.Empty();
}


UNavigationTargetSubsystem* UNavigationTargetSubsystem::Get(const UWorld* World)
{
	if (IsValid(World))
		return  World->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	return 	nullptr;
}

TArray<FVector> UNavigationTargetSubsystem::GetAllActiveTargetLocations()
{
	TArray<FVector> Locations{};
	ActorToTarget.GenerateValueArray(Locations);
	return  Locations;
}

bool UNavigationTargetSubsystem::HasCloseNavTarget(const FVector& Location, float RadiusToCheck, const AActor* Actor) const
{
	
	for (const TTuple<AActor*,FVector>  Target: ActorToTarget)
	{
		if (Target.Key == Actor)
			continue;
		
		const float Distance = FVector::Distance(Location, Target.Value);
		if (Distance<=RadiusToCheck)
		{
			return true;
		}
	}
	return false;
}

