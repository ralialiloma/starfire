// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Sf_CoverGenComponent.generated.h"


UCLASS(ClassGroup=AI, meta=(BlueprintSpawnableComponent))
class STARFIRE_API USf_CoverGenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USf_CoverGenComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<FVector> GetCoverLocations() const;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<FVector> GetPeakLocations() const;
protected:
	void StartRunQuery();
	void DoRunQuery();
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void GetQueryLocations(TArray<FVector>& QueryLocations) const;
	const FEnvQueryResult* GetQueryResult() const;

	void UpdateCoverAndPeakLocations(float MinValue= 0.1f);
	void FindPeakLocations(TArray<FVector>& PeakLocations) const;

	void DebugCoverLocations() const;
	void DebugPeakLocations() const;
#pragma endregion


#pragma region Properties
public:
protected:
	UPROPERTY(EditDefaultsOnly, meta = (CustomConfig))
	UEnvQuery* CoverEnvQuery;

	UPROPERTY(EditDefaultsOnly, meta = (CustomConfig))
	float QueryUpdateRate = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	UEnvQueryInstanceBlueprintWrapper* CoverQueryInstance;

private:
	UPROPERTY()
	TArray<FVector> SavedCoverLocations;
	UPROPERTY()
	TArray<FVector> SavedPeakLocations;

	UPROPERTY()
	TMap<FVector,FVector> SavedPeakCoverPair;
#pragma endregion

};
