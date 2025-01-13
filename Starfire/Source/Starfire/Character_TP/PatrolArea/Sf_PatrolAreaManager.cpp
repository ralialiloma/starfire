// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PatrolAreaManager.h"

#include "Kismet/GameplayStatics.h"


USf_PatrolAreaManager::USf_PatrolAreaManager()
{
}

void USf_PatrolAreaManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

USf_PatrolAreaManager* USf_PatrolAreaManager::Get(const UWorld* World)
{
	if (!IsValid(World))
		return nullptr;

	if (!IsValid(World->GetGameInstance()) )
		return nullptr;

	return  World->GetSubsystem<USf_PatrolAreaManager>();
}

void USf_PatrolAreaManager::RegisterPatrolArea(ASf_PatrolArea* PatrolArea)
{
	if (IsValid(PatrolArea) && !RegisteredPatrolAreas.Contains(PatrolArea))
		RegisteredPatrolAreas.AddUnique(PatrolArea);
}

void USf_PatrolAreaManager::UnregisterPatrolArea(ASf_PatrolArea* PatrolArea)
{
	RegisteredPatrolAreas.Remove(PatrolArea);
}

ASf_PatrolArea* USf_PatrolAreaManager::RegisterMarker(ASf_PatrolAreaMarker* MarkerToRegister)
{

	if (!IsValid(MarkerToRegister))
		return nullptr;
	bool bFound;
	ASf_PatrolArea* FoundArea  =  FindPatrolAreaForLocation(MarkerToRegister->GetActorLocation(), bFound);
	if (!IsValid(FoundArea))
		return nullptr;
		
	FoundArea->TryRegisterMarker(MarkerToRegister);
	return FoundArea;
}

void USf_PatrolAreaManager::UnregisterMarker(ASf_PatrolAreaMarker* MarkerToUnregister)
{
	if (!IsValid(MarkerToUnregister))
		return;

	for (ASf_PatrolArea* PatrolArea: RegisteredPatrolAreas)
	{
		if (!IsValid(PatrolArea))
			continue;
		PatrolArea->UnregisterMarker(MarkerToUnregister);
	}
}

ASf_PatrolArea* USf_PatrolAreaManager::FindPatrolAreaForLocation(const FVector PointToTest, bool &bFound)
{
	bFound = false;
	for (ASf_PatrolArea* PatrolArea: RegisteredPatrolAreas)
	{
		if (!IsValid(PatrolArea))
			continue;

		if (PatrolArea->IsInBox(PointToTest))
		{
			bFound = true;
			return PatrolArea;
		}
	}
	return nullptr;
}

bool USf_PatrolAreaManager::IsPointInAnyPatrolArea(const FVector& PointToTest)
{
	bool bFound = false;
	FindPatrolAreaForLocation(PointToTest, bFound);
	return bFound;
}

TArray<ASf_PatrolArea*> USf_PatrolAreaManager::GetFreePatrolAreas() const
{
	TArray<ASf_PatrolArea*> PatrolAreas = RegisteredPatrolAreas;

	for (auto PatrolArea : RegisteredPatrolAreas)
	{
		if (!PatrolArea)
			continue;
		
		if (PatrolArea->IsOccupied())
			PatrolAreas.Remove(PatrolArea);
		
		if (PatrolArea->IsIndependent())
			PatrolAreas.Remove(PatrolArea);
	}
	
	return PatrolAreas;
}

TArray<ASf_PatrolArea*> USf_PatrolAreaManager::GetAllRegisteredPatrolAreas() const
{
	return RegisteredPatrolAreas;
}

TArray<ASf_PatrolArea*> USf_PatrolAreaManager::GetAllPatrolAreas(const UObject* WorldContext)
{
	TArray<ASf_PatrolArea*> PatrolAreas {};
	TArray<AActor*> FoundActors {};
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASf_PatrolArea::StaticClass(), FoundActors);
	for (auto Area : FoundActors)
	{
		ASf_PatrolArea* PatrolArea = Cast<ASf_PatrolArea>(Area);
		
		if (!PatrolArea)
			continue;
		
		if (!PatrolArea->IsIndependent())
			PatrolAreas.AddUnique(PatrolArea);
	}
	return PatrolAreas;
}
