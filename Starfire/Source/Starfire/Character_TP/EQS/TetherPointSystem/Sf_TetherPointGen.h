﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Sf_TetherPointGen.generated.h"


UCLASS()
class UTetherPoint: public UObject
{
	GENERATED_BODY()

public:
	UTetherPoint();

	bool operator==(const UTetherPoint& Other) const;

	bool operator!=(const UTetherPoint& Other) const;

public:
	UPROPERTY()
	FVector CenterLocation;
	
	UPROPERTY()
	float VerticalOffset;

	UPROPERTY()
	TArray<FVector> SurroundingPoints;

	UPROPERTY()
	float CoverPotential;

	UPROPERTY()
	float DistanceToWall = 0;

	UPROPERTY()
	FGameplayTagContainer GameplayTags;

	UPROPERTY()
	float Scale;

	UPROPERTY()
	FVector Size;

	void FindSurroundingPoints(const UObject* WorldContextObject);

	void UpdateScore(const FVector& PlayerLocation, const UObject* WorldContextObject, const TArray<AActor*>& IgnoredActors, ETraceTypeQuery TraceTypeQuery);
};

UCLASS(Blueprintable)
class STARFIRE_API ASf_TetherPointGen : public AActor
{
	GENERATED_BODY()

public:
	ASf_TetherPointGen();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
#pragma region Functions
public:
	void GeneratePoints();

	static ASf_TetherPointGen* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetCoverLocations(float MinScore = 0.5f) const;

	TArray<UTetherPoint*>  GetAllTetherPoints();

	UFUNCTION(BlueprintCallable)
	bool VerifyCover(FVector LocationToVerify, float Extent, const float MinScore = 0.5f) const;

	UFUNCTION(BlueprintCallable)
	bool VerifyPeak(FVector LocationToVerify, float Extent, const float MaxScore = 0.1f) const;

	UFUNCTION(BlueprintCallable)
	void GetClosestCoverTo(const FVector Location, const float MinScore, float& OutDistance, FVector& ClosestCoverLocation, bool& bFound) const;

	UFUNCTION(BlueprintCallable) 
	void GetClosestPeakTo(FVector Location, float MaxScore, float& OutDistance, FVector& ClosestCoverLocation, bool& bFound) const;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPeakLocationsInRange(float MaxScore = 0.1f) const;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetPeakLocationsInRadius(const float MaxScore, const FVector& Location, float Radius) const;

	UFUNCTION(BlueprintCallable)
	TArray<UTetherPoint*> GetRelevantTetherPoints() const;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetRelevantActors() const;

	UFUNCTION(BlueprintCallable)
	void RegisterActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void UnregisterActor(AActor* Actor);
	
private:
	void UpdateTetherPoints();
	void AddRelevantTetherPointsToProcess();
#pragma endregion


#pragma region Properties
public:
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UTetherPoint*> AllTetherPoints;

	UPROPERTY(BlueprintReadOnly)
	TArray<UTetherPoint*> TetherPointsToProcess;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float Scale = 300.f;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FVector Size = FVector(30,30,9);

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float TetherScale = 50.f;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FVector TetherPointSize = FVector(2,2,2);

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int AmountOfUpdatedTetherPointsPerFrame = 10;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery>  TraceTypeQuery;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>>  ObjectTypeQuery;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float VerticalOffset = 50;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float PlayerUpdateRateInSeconds = 1;

	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MaxWallDistance = 200;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MaxRelevancyDistance = 1000.f;
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MinActorUpdateDistance = 200;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	float MaxActorUpdateDistance = 2000;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UNavigationQueryFilter> QueryFilterClass;

private:
	UPROPERTY()
	FTimerHandle CloseToPlayerTethers;

	UPROPERTY()
	FTimerHandle OtherTethers;

	UPROPERTY()
	TArray<AActor*> RegisteredActors;
#pragma endregion

};