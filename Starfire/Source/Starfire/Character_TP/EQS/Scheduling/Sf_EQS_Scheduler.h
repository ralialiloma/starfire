// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Tickable.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Sf_EQS_Scheduler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEQSResult, const TArray<FVector>&, Results, FGuid, UniqueID);


USTRUCT(BlueprintType)
struct FScheduledEnvRequest
{
	GENERATED_BODY()

public:
	FScheduledEnvRequest(UObject* InQuerier, UEnvQuery* InQueryTemplate, TEnumAsByte<EEnvQueryRunMode::Type> InRunMode);

	FScheduledEnvRequest();

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* Querier = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UEnvQuery* QueryTemplate = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EEnvQueryRunMode::Type>  RunMode;

	UPROPERTY()
	FGuid UniqueIdentifier = FGuid::NewGuid();
};



UCLASS(BlueprintType, Blueprintable)
class STARFIRE_API USf_EQS_Scheduler : public UGameInstanceSubsystem 
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static USf_EQS_Scheduler* GetCurrent(const UWorld* World);
	
	UFUNCTION(BlueprintCallable)
	FGuid ScheduleRequest(FScheduledEnvRequest ScheduledEnvRequest);

	UFUNCTION(BlueprintCallable)
	bool HasExistingSimilarRequest(FScheduledEnvRequest RequestToCompare, FScheduledEnvRequest OutFoundSimilarRequests);

	bool Tick();
protected:
	void RunRequest(const FScheduledEnvRequest& Request);

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
public:
	UPROPERTY(BlueprintAssignable)
	FOnEQSResult OnEQSResult;
protected:
	UPROPERTY()
	int MaxAllowedActiveRequests = 3;
	
	UPROPERTY()
	TArray<FScheduledEnvRequest> ScheduledRequests;

	UPROPERTY()
	TMap<UEnvQueryInstanceBlueprintWrapper*,FScheduledEnvRequest> ActiveRequests;

	UPROPERTY()
	float Tickrate = 0.5f;
	
	FTSTicker::FDelegateHandle TickHandle;

};


