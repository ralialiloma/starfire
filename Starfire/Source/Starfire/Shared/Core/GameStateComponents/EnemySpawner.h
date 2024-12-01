// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerComponent.h"
#include "ResourceSpawner.h"
#include "Components/ActorComponent.h"
#include "EnemySpawner.generated.h"


class ASf_TP_Character;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARFIRE_API UEnemySpawner : public UManagerComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void RequestNewEnemy();

protected:

	virtual void StartGame() override;

	// void SpawnEnemiesForVein(FResourceVein Vein);
	void SpawnEnemy(const FTransform& Transform);
	void SpawnEnemyRandom();
	void RegisterEnemy(ASf_TP_Character* Enemy);

	TSubclassOf<ASf_TP_Character> EvaluateSpawnedEnemyClass() const;
	TArray<FTransform> GetViableSpawns() const;
	FTransform GetSpawnLocation() const;
	TArray<ASf_TP_Character*> GetAllEnemies() const;

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<ASf_TP_Character> SpawnedEnemyClass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int MaxEnemies = 10;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SpawnDelay = 5.f;
	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// int EnemiesPerVein = 3;
	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	// bool bWaitForVeins = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (EditCondition = "bWaitForVeins"))
	bool bStartWithMax = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bDisableSpawning = true;
	
	UPROPERTY()
	FTimerHandle SpawnEnemyTimerHandle = FTimerHandle();
	UPROPERTY()
	int EnemiesToBeSpawned = 0;

};
