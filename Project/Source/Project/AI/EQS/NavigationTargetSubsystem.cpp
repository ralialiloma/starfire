

#include "NavigationTargetSubsystem.h"



void UNavigationTargetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UNavigationTargetSubsystem::Deinitialize()
{
	Super::Deinitialize();
}


//Reserved Covers
void UNavigationTargetSubsystem::RegisterReservedCover(FVector CoverLocation)
{
	ReservedCovers.Add(CoverLocation);
}

void UNavigationTargetSubsystem::UnregisterReservedCover(FVector CoverLocation)
{
	ReservedCovers.Remove(CoverLocation);
}

void UNavigationTargetSubsystem::ClearReservedCovers()
{
	ReservedCovers.Empty();
}

TArray<FVector> UNavigationTargetSubsystem::GetAllReservedCovers()
{
	return  ReservedCovers;
}

