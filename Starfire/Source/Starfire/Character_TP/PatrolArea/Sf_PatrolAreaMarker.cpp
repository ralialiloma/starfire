// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PatrolAreaMarker.h"
#include "NavigationSystem.h"


ASf_PatrolAreaMarker::ASf_PatrolAreaMarker()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASf_PatrolAreaMarker::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ProjectToNavigation();
	
}

void ASf_PatrolAreaMarker::ProjectToNavigation()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!IsValid(NavSys))
	{
		UE_LOG(LogTemp, Warning, TEXT("Navigation system not found!"));
		return;
	}
	
	FNavLocation ProjectedLocation;
	bONavmesh = NavSys->ProjectPointToNavigation(
		GetActorLocation(),
		ProjectedLocation,
		FVector(100.0f, 100.0f, 200.0f),
		nullptr
	);

	if (bONavmesh)
	{
		SetActorLocation(ProjectedLocation.Location);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to project point to navigation mesh!"));
	}
}

