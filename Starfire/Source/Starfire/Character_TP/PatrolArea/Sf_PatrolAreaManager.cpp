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

ASf_PatrolArea* USf_PatrolAreaManager::RegisterMarker(const FGuid Guid,ASf_PatrolAreaMarker* MarkerToRegister)
{
	if (!IsValid(MarkerToRegister))
		return nullptr;
	ASf_PatrolArea* FoundArea  =  FindPatrolAreaForLocation(MarkerToRegister->GetActorLocation());
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

ASf_PatrolArea* USf_PatrolAreaManager::FindPatrolAreaForLocation(const FVector PointToTest)
{
	for (ASf_PatrolArea* PatrolArea: RegisteredPatrolAreas)
	{
		if (!IsValid(PatrolArea))
			continue;

		if (PatrolArea->IsInBox(PointToTest))
			return PatrolArea;
	}

	return nullptr;
}

bool USf_PatrolAreaManager::IsPointInAnyPatrolArea(const FVector& PointToTest)
{
	return FindPatrolAreaForLocation(PointToTest)!=nullptr;
}
