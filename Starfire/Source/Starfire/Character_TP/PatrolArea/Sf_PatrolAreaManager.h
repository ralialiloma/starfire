// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sf_PatrolArea.h"
#include "Sf_PatrolAreaManager.generated.h"

UCLASS(BlueprintType)
class STARFIRE_API USf_PatrolAreaManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	USf_PatrolAreaManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	static USf_PatrolAreaManager* Get(const UWorld* World);

	void RegisterPatrolArea(ASf_PatrolArea* PatrolArea);
	void UnregisterPatrolArea (ASf_PatrolArea* PatrolArea);

	ASf_PatrolArea* RegisterMarker(ASf_PatrolAreaMarker* MarkerToRegister);
	void UnregisterMarker (ASf_PatrolAreaMarker* MarkerToUnregister);

	UFUNCTION(BlueprintCallable)
	ASf_PatrolArea* FindPatrolAreaForLocation(const FVector PointToTest, bool& bFound);

	UFUNCTION(BlueprintCallable)
	bool IsPointInAnyPatrolArea(const FVector& PointToTest);

	UFUNCTION(BlueprintCallable)
	TArray<ASf_PatrolArea*> GetFreePatrolAreas() const;
	UFUNCTION(BlueprintCallable)
	TArray<ASf_PatrolArea*> GetAllRegisteredPatrolAreas() const;
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static TArray<ASf_PatrolArea*> GetAllPatrolAreas(const UObject* WorldContext);

protected:

	UPROPERTY()
	TArray<ASf_PatrolArea*> RegisteredPatrolAreas;
	
};