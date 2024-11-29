// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sf_PatrolArea.h"
#include "Sf_PatrolAreaManager.generated.h"

UCLASS(BlueprintType)
class STARFIRE_API USf_PatrolAreaManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USf_PatrolAreaManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	static USf_PatrolAreaManager* Get(const UWorld* World);

	void RegisterPatrolArea(ASf_PatrolArea* PatrolArea);
	void UnregisterPatrolArea (ASf_PatrolArea* PatrolArea);

	UFUNCTION(BlueprintCallable)
	bool IsPointInAnyPatrolArea(const FVector& PointToTest);

protected:

	UPROPERTY()
	TArray<ASf_PatrolArea*> RegisteredPatrolAreas;
	
};