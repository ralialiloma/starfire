// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "Sf_PatrolAreaMarker.generated.h"

class ASf_PatrolArea;

UCLASS(BlueprintType)
class STARFIRE_API ASf_PatrolAreaMarker: public AActor
{
	GENERATED_BODY()

public:
	ASf_PatrolAreaMarker();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

#pragma region Functions
public:
	UFUNCTION()
	bool ShouldBeTetherPoint() const;
protected:
	void ProjectToNavigation();

#pragma endregion
	

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = PatrolArea,meta=(Categories="Gameplay.PatrolAreaMarkerTypes"))
	FGameplayTagContainer PatrolTags;
protected:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UStaticMeshComponent* CollisionMesh;
	UPROPERTY()
	bool bONavmesh = false;
	UPROPERTY()
	ASf_PatrolArea* OwningPatrolArea = nullptr;
	UPROPERTY()
	FGuid Guid;
#pragma endregion
	

};