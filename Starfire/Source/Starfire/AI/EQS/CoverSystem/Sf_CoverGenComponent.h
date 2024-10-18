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
	void GetCoverLocations(TArray<FVector>& CoverLocations) const;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	void GetPeakLocations(TArray<FVector>& PeakLocations) const;
protected:
	void StartRunQuery();
	void DoRunQuery();
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void GetQueryLocations(TArray<FVector>& QueryLocations) const;
	const FEnvQueryResult* GetQueryResult() const;

	void DebugCoverLocations() const;
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
#pragma endregion

};
