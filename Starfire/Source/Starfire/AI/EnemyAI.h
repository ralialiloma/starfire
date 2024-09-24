// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFeature.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

UCLASS()
class STARFIRE_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UEnemyFeature*> Features;
	
public:
	// Sets default values for this character's properties
	AEnemyAI();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);

	UFUNCTION(BlueprintCallable, Category = "EnemyFeatures")
	bool TryAddFeature(UEnemyFeature* Feature);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AIDeviations")
	FVector CalculatePositionDeviation(FVector Position, float Accuracy);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "AIDeviations")
	FRotator CalculateRotationDeviation(FRotator Rotation, float Accuracy);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIDeviations")
	float CalculateDeviationValue(float ValuetoDeviateFrom, float Accuracy);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
