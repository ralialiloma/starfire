// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerActor.h"
#include "ResourceSpawner.h"
#include "Components/ActorComponent.h"
#include "EnemySpawner.generated.h"

class ASf_PatrolArea;
class ASf_TP_Character;
DECLARE_LOG_CATEGORY_EXTERN(LogEnemySpawner, Log, All);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), PrioritizeCategories = "Settings")
class STARFIRE_API AEnemySpawner : public AManagerActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void RequestNewEnemy();

protected:

	virtual void StartGame() override;
	void DelayedStartGame();

	// void SpawnEnemiesForVein(FResourceVein Vein);
	void SpawnEnemy(const FTransform& Transform);
	void SpawnEnemyRandom();
	void RegisterEnemy(ASf_TP_Character* Enemy);

	TSubclassOf<ASf_TP_Character> EvaluateSpawnedEnemyClass() const;
	TArray<ASf_PatrolArea*> GetViableSpawns() const;
	FTransform GetSpawnLocation() const;
	TArray<ASf_TP_Character*> GetAllEnemies() const;

	FString RequestEnemyName();
	static FString GetIncrementedName(FString Name);

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	TSubclassOf<ASf_TP_Character> SpawnedEnemyClass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	int MaxEnemies = 10;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	float SpawnDelay = 5.f;
	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	// int EnemiesPerVein = 3;
	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	// bool bWaitForVeins = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	bool bStartWithMax = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	float MinSpawnDistance = 1000.f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	bool bDisableSpawning = true;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Settings)
	TArray<FString> PossibleEnemyNames { 
		FString("Tony"), 
		FString("Victor"), 
		FString("Lena"), 
		FString("Max"), 
		FString("Sasha"), 
		FString("Ivy"), 
		FString("Dimitri"), 
		FString("Raven"), 
		FString("Kira"), 
		FString("Axel"),
		FString("Serena"),
		FString("Elio"),
		FString("Maxine"),
		FString("Richard"),
		FString("Samantha"),
		FString("Jesse"),
		FString("Marlon"),
		FString("Lukas"),
		FString("Jermaine"),
		FString("Marek")
	};
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> UsedEnemyNames {};
	UPROPERTY()
	int TonyCount = 0;
	
	UPROPERTY()
	FTimerHandle SpawnEnemyTimerHandle = FTimerHandle();
	UPROPERTY()
	int EnemiesToBeSpawned = 0;

};
