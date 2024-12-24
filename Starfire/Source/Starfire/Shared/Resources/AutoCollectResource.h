// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resource.h"
#include "AutoCollectResource.generated.h"

class USphereComponent;

UCLASS()
class STARFIRE_API AAutoCollectResource : public AResource
{
	GENERATED_BODY()

public:
	
	AAutoCollectResource();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	bool IsPlayerContained() const;

protected:

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	float EvaluateCurveAt(float Value) const;

	//Subscriptions
	UFUNCTION()
	void OnOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CollectRange = 200.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float PickupRange = 20.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UCurveFloat* CollectCurve = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bPhysicsBased = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "!bPhysicsBased"))
	float InterpSpeed = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bPhysicsBased"))
	float CollectForce = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bPhysicsBased"))
	float CollectVerticalForce = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bPhysicsBased"))
	float DistancePower = 2;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bPhysicsBased"))
	float DistanceThreshold = 100;
	
	UPROPERTY(BlueprintReadOnly)
	APawn* ContainedPawn = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector PreviousVectorToPlayer = FVector::Zero();
};
