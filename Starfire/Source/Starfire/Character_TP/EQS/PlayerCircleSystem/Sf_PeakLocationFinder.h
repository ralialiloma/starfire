// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sf_PeakLocationFinder.generated.h"

USTRUCT()
struct FSf_Circle
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Radius;

	UPROPERTY()
	TArray<FVector> PointsOnCircle;
	
};

USTRUCT()
struct FSf_ActorAssignment
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* AssignedActor;

	UPROPERTY()
	FVector TargetPosition;
};


UCLASS(Blueprintable)
class STARFIRE_API USf_PeakLocationFinder : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	USf_PeakLocationFinder(const FObjectInitializer& ObjectInitializer);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Tick();
	static USf_PeakLocationFinder* GetCurrent(const UWorld* World);
	
	UFUNCTION(BlueprintCallable)
	void RegisterActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void UnregisterActor(AActor* Actor);
	FVector GetTargetLocationForActor(const AActor* Actor) const;

protected:
	TArray<FSf_Circle> GenerateCircleAroundPlayer(const int AmountOfRings, const int PointsPerRing, const float RingDistance, const int MinRingSize, const float MaxHeightDifference) const;
	void DrawDebugCircles(
		const TArray<FSf_Circle>& Circles,
		UWorld* World,
		const FColor& CircleColor,
		const FColor& SphereColor,
		float SphereRadius, float
		Duration);
	void UpdateActorAssignments();

protected:
	UPROPERTY()
	float MaxApproachAngle = 60.f;
	
	UPROPERTY()
	TArray<FSf_ActorAssignment> RegisteredActors;
	UPROPERTY()
	TArray<FSf_Circle> GeneratedCircles;

	
	UPROPERTY()
	float Tickrate = 0.2f;
	UPROPERTY()
	FTimerHandle TickTimerHandle;
};
