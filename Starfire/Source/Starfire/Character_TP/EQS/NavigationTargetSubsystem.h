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
	TArray<FVector> GetAllReservedCovers();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasCloseNavTarget(FVector Location, float RadiusToCheck = 200) const;

	UFUNCTION(BlueprintCallable)
	void RegisterNavTarget(FVector CoverLocation);

	UFUNCTION(BlueprintCallable)
	void UnregisterNavTarget(FVector CoverLocation);
	
	void ClearReservedCovers();

	static UNavigationTargetSubsystem* Get(const UWorld* World);

protected:
	void DebugTick();

protected:
	UPROPERTY()
	TArray<FVector> ActiveNavTargets;

	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UPROPERTY()
	float Tickrate = 0.5f;

};
