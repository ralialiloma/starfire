// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Tickable.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SF_EQS_Scheduler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEQSResult, const TArray<FVector>&, Results, FGuid, UniqueID);


USTRUCT(BlueprintType)
struct FScheduledEnvRequest
{
	GENERATED_BODY()

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
	
	UFUNCTION(BlueprintCallable)
	FGuid ScheduleRequest(FScheduledEnvRequest ScheduledEnvRequest);

	bool Tick(float DeltaTime);



protected:
	void RunRequest(const FScheduledEnvRequest& Request);

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);



public:
	UPROPERTY(BlueprintAssignable)
	FOnEQSResult OnEQSResult;
protected:
	UPROPERTY()
	TArray<FScheduledEnvRequest> ScheduledRequests;

	UPROPERTY()
	TMap<UEnvQueryInstanceBlueprintWrapper*,FScheduledEnvRequest> ActiveRequests;
	
	FTSTicker::FDelegateHandle TickHandle;

};


