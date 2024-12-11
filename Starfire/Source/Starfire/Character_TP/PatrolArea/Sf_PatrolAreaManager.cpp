// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PatrolAreaManager.h"


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

	return  World->GetGameInstance()->GetSubsystem<USf_PatrolAreaManager>();
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
	}
	
	return PatrolAreas;
}
