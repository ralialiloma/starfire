// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Character_TP/EQS/Scheduling/Sf_EQS_Scheduler.h"
#include "Sf_CloseToPlayerLoc.generated.h"


USTRUCT()
struct FCloseToPlayerLocEntry
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Actor;

	UPROPERTY()
	FVector RegisterdCloseToPlayerPoint;

	UPROPERTY()
	int AssignedIndex = -1;
};

UCLASS(Blueprintable)
class STARFIRE_API USf_CloseToPlayerLoc : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	USf_CloseToPlayerLoc(const FObjectInitializer& ObjectInitializer);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Tick();
	static USf_CloseToPlayerLoc* GetCurrent(const UWorld* World);
	TArray<FVector> GetCurrentCloseToPlayerLocations();

	void RegisterActor(AActor* ActorToRegister);
	void UnregisterActor(AActor* ActorToUnregister);
	FVector GetCloseToPlayerLoc(const AActor* ActorToFindLocationFor);
	bool ValidateCloseToPlayerLoc(FVector Location, float Radius);
	FVector GetRegisterdActorLocation(const AActor* Actor);
	FCloseToPlayerLocEntry GetEntryByActor(const AActor* Actor);

protected:
	UFUNCTION()
	void OnEQSResult(const TArray<FVector>& Results, FGuid UniqueID);

	UFUNCTION()
	void UpdateQuery();

	void UpdateEntries();

protected:
	UPROPERTY()
	float Tickrate = 0.2f;

	UPROPERTY()
	FGuid ScheduledEQSID;

	UPROPERTY()
	UEnvQuery* Query;

	UPROPERTY()
	ASf_FP_Character* Player;

	UPROPERTY()
	TArray<FVector> CurrentCloseToPlayerLocations = TArray<FVector>{};

	UPROPERTY()
	bool bFinishedQuery;

	UPROPERTY()
	TArray<AActor*> RegisteredActors;

	TArray<FCloseToPlayerLocEntry> Entries;

private:
	UPROPERTY()
	USf_EQS_Scheduler* Scheduler;
	
	UPROPERTY()
	FTimerHandle TickTimerHandle;
};
