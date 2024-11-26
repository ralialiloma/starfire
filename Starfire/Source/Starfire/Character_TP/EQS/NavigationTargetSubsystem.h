// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationTargetSubsystem.generated.h"


UCLASS(BlueprintType, Blueprintable)
class STARFIRE_API UNavigationTargetSubsystem : public UGameInstanceSubsystem 
{
	GENERATED_BODY()

public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FVector> GetAllActiveTargetLocations();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasCloseNavTarget(const FVector& Location, float RadiusToCheck = 200.f, const AActor* Actor = nullptr) const;
	

	UFUNCTION(BlueprintCallable)
	void RegisterNavTarget(AActor* PursuingAgent ,FVector LocationToRegister);

	UFUNCTION(BlueprintCallable)
	void UnregisterNavTarget(AActor* PursuingAgent);
	
	void ClearReservedCovers();

	static UNavigationTargetSubsystem* Get(const UWorld* World);

protected:
	void DebugTick();

protected:
	//UPROPERTY()
	//TArray<FVector> ActiveNavTargets;

	UPROPERTY()
	TArray<AActor*> Navblockers;

	UPROPERTY()
	TMap<AActor*,FVector> ActorToTarget;
	
	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UPROPERTY()
	float Tickrate = 0.5f;

};
