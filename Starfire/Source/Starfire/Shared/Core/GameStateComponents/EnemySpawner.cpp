// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Features/Death/CF_Death.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolAreaManager.h"

void UEnemySpawner::StartGame()
{
	Super::StartGame();

	if (bDisableSpawning)
		return; //Temp
	// if (bWaitForVeins)
	// 	return;

	TArray<ASf_TP_Character*> Enemies = GetAllEnemies();
	if (Enemies.Num() < MaxEnemies)
	{
		if (bStartWithMax)
		{
			for (int i = Enemies.Num(); i < MaxEnemies; ++i)
			{
				SpawnEnemyRandom();
			}
		}
		else
		{
			EnemiesToBeSpawned = MaxEnemies - Enemies.Num();
			RequestNewEnemy();
		}
	}
	for (auto Enemy : Enemies)
	{
		RegisterEnemy(Enemy);
	}
}

void UEnemySpawner::SpawnEnemy(const FTransform& Transform)
{
	UE_LOG(LogTemp, Log, TEXT("SPawning Enemy."));

	UClass* EnemyClass = EvaluateSpawnedEnemyClass();
	if (!EnemyClass)
	{
		//Print Error
		return;
	}
	
	ASf_TP_Character* Enemy = GetWorld()->SpawnActor<ASf_TP_Character>(EvaluateSpawnedEnemyClass(), Transform.GetLocation(), Transform.GetRotation().Rotator());
	if(Enemy && Enemy->GetFeatureByClass<UCF_Death>())
	{
		Enemy->GetFeatureByClass<UCF_Death>()->OnDeath_CPP.AddLambda([this]()
		{
			RequestNewEnemy();
		});
	}
	
}

void UEnemySpawner::SpawnEnemyRandom()
{
	SpawnEnemy(GetSpawnLocation());
}

void UEnemySpawner:: RegisterEnemy(ASf_TP_Character* Enemy)
{
	UE_LOG(LogTemp, Log, TEXT("Register Enemy"));

	if(Enemy && Enemy->GetFeatureByClass<UCF_Death>())
	{
		Enemy->GetFeatureByClass<UCF_Death>()->OnDeath_CPP.AddLambda([this]()
		{
			RequestNewEnemy();
		});
	}
}

void UEnemySpawner::RequestNewEnemy()
{
	if (EnemiesToBeSpawned <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyTimerHandle);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("New Enemy Request process"));

	SpawnEnemyRandom();
	EnemiesToBeSpawned--;

	if (EnemiesToBeSpawned > 0)
	{
		if (!SpawnEnemyTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimerHandle,this, &UEnemySpawner::RequestNewEnemy, SpawnDelay);
	}
}

TSubclassOf<ASf_TP_Character> UEnemySpawner::EvaluateSpawnedEnemyClass() const
{
	return SpawnedEnemyClass;
}

TArray<FTransform> UEnemySpawner::GetViableSpawns() const
{
	TArray<ASf_PatrolArea*> Areas = USf_PatrolAreaManager::Get(this->GetWorld())->GetFreePatrolAreas();
	TArray<FTransform> AreaTransfroms;
	for (auto Area : Areas)
	{
		if (Area)
			AreaTransfroms.Add(Area->GetTransform());
	}
	
	return AreaTransfroms;
}

FTransform UEnemySpawner::GetSpawnLocation() const
{
	TArray<FTransform> PossibleTransforms = GetViableSpawns();
	if (PossibleTransforms.Num() <= 0)
		return FTransform();
	
	//TODO: Currently chooses a random location
	
	int RandomIndex = FMath::RandRange(0, PossibleTransforms.Num() - 1);
	return PossibleTransforms[RandomIndex];
}

TArray<ASf_TP_Character*> UEnemySpawner::GetAllEnemies() const
{
	TArray<AActor*> FoundActors {};
	TArray<ASf_TP_Character*> Enemies {};
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASf_TP_Character::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (ASf_TP_Character* Enemy = Cast<ASf_TP_Character>(Actor))
		{
			Enemies.Add(Enemy);
		}
	}

	return Enemies;
}
