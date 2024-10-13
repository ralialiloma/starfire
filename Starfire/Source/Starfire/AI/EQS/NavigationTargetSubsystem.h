// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationTargetSubsystem.generated.h"


UCLASS(BlueprintType, Blueprintable)
class STARFIRE_API UNavigationTargetSubsystem : public UGameInstanceSubsystem 
{
	GENERATED_BODY()

private:
	TArray<FVector> ReservedCovers;
	
public:	
	virtual  void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FVector> GetAllReservedCovers();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool LocationInReservedCover(FVector Location, float RadiusToCheck = 200) const;

	UFUNCTION(BlueprintCallable)
	void RegisterReservedCover(FVector CoverLocation);

	UFUNCTION(BlueprintCallable)
	void UnregisterReservedCover(FVector CoverLocation);
	
	void ClearReservedCovers();
};
