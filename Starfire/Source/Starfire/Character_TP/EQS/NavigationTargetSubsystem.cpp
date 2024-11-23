

#include "NavigationTargetSubsystem.h"

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
	for (FVector Target: ActiveNavTargets)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Target,150.f,6,FColor::Blue,Tickrate+0.01f,2);
	}
}


//Reserved Covers
void UNavigationTargetSubsystem::RegisterNavTarget(FVector CoverLocation)
{
	ActiveNavTargets.Add(CoverLocation);
}

void UNavigationTargetSubsystem::UnregisterNavTarget(FVector CoverLocation)
{
	ActiveNavTargets.Remove(CoverLocation);
}

void UNavigationTargetSubsystem::ClearReservedCovers()
{
	ActiveNavTargets.Empty();
}

UNavigationTargetSubsystem* UNavigationTargetSubsystem::Get(const UWorld* World)
{
	if (IsValid(World))
		return  World->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	return 	nullptr;
}

TArray<FVector> UNavigationTargetSubsystem::GetAllReservedCovers()
{
	return  ActiveNavTargets;
}

bool UNavigationTargetSubsystem::HasCloseNavTarget(const FVector Location, float RadiusToCheck) const
{
	for (auto ReservedCover: ActiveNavTargets)
	{
		const float Distance = FVector::Distance(Location, ReservedCover);
		if (Distance<=RadiusToCheck)
		{
			return true;
		}
	}
	return false;
}

