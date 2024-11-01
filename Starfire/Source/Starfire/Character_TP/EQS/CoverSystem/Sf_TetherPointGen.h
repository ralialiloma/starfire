// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
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
	float Score;

	UPROPERTY()
	float DistanceToWall = 0;

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

	UFUNCTION(Blueprintable)
	TArray<FVector> GetCoverLocations(float MinScore = 0.5f) const;
	
	UFUNCTION(Blueprintable)
	TArray<FVector> GetPeakLocations(float MaxScore = 0.1f) const;
private:
	void UpdateTetherPoints();
	void AddCloseToPlayerTetherPointsToProcess();
	TArray<UTetherPoint*> GetTetherPointsAroundPlayer() const;
#pragma endregion


#pragma region Properties
public:
protected:
	UPROPERTY()
	TArray<UTetherPoint*> AllTetherPoints;

	UPROPERTY()
	TArray<UTetherPoint*> TetherPointsToProcess;

	UPROPERTY()
	float Scale = 300.f;

	UPROPERTY()
	FVector Size = FVector(30,30,9);

	UPROPERTY()
	float TetherScale = 50.f;

	UPROPERTY()
	FVector TetherPointSize = FVector(2,2,2);

	UPROPERTY()
	int AmountOfUpdatedTetherPointsPerFrame = 10;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TEnumAsByte<ETraceTypeQuery>  TraceTypeQuery;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>>  ObjectTypeQuery;

	UPROPERTY()
	float VerticalOffset = 50;

	UPROPERTY()
	float OtherUpdateRateInSecdonds = 2;

	UPROPERTY()
	float PlayerUpdateRateInSeconds = 1;


	UPROPERTY()
	float MaxWallDistance = 200;
	
	//Player
	UPROPERTY()
	float MinUpdateDistance = 200;

	UPROPERTY()
	float MaxUpdateDistance = 2000;
private:
	UPROPERTY()
	FTimerHandle CloseToPlayerTethers;

	UPROPERTY()
	FTimerHandle OtherTethers;
#pragma endregion

};
