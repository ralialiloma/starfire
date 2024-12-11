// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Features/Death/CF_Death.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolAreaManager.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogEnemySpawner);

void UEnemySpawner::StartGame()
{
	Super::StartGame();

	if (bDisableSpawning)
		return;

	// Delay the execution by 1 frame
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		DelayedStartGame();
	});
}

void UEnemySpawner::DelayedStartGame()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASf_PatrolArea::StaticClass(), OutActors);
	if (MaxEnemies > OutActors.Num())
	{
		SF_SIMPLE_DEBUG(
			LogEnemySpawner,
			Warning,
			FColor::Orange,
			*FString::Printf(TEXT("MaxEnemies changed from %i to %i"), MaxEnemies, OutActors.Num()),
			Spawning::Enemies);

		MaxEnemies = OutActors.Num();
	}

	// if (bWaitForVeins)
	//     return;

	TArray<ASf_TP_Character*> Enemies = GetAllEnemies();
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::FromInt(Enemies.Num()));
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
    SF_SIMPLE_DEBUG(
        LogEnemySpawner,
        Warning,
        FColor::White,
        *FString::Printf(TEXT("Spawning Enemy")),
        Spawning::Enemies);

    UClass* EnemyClass = EvaluateSpawnedEnemyClass();
    if (!EnemyClass)
    {
        SF_SIMPLE_DEBUG(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Spawning Class Invalid!")),
            Spawning::Enemies);
        return;
    }

    FActorSpawnParameters SpawnParameters{};
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Retrieve default object to access capsule size
    const ASf_TP_Character* DefaultEnemy = EnemyClass->GetDefaultObject<ASf_TP_Character>();
    if (!DefaultEnemy)
    {
        SF_SIMPLE_DEBUG(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Failed to retrieve default object for enemy class!")),
            Spawning::Enemies);
        return;
    }

    // Get capsule component half-height
    const UCapsuleComponent* Capsule = DefaultEnemy->GetCapsuleComponent();
    if (!Capsule)
    {
        SF_SIMPLE_DEBUG(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Enemy class does not have a capsule component!")),
            Spawning::Enemies);
        return;
    }

    float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
    FVector AdjustedLocation = Transform.GetLocation()+FVector::UpVector*CapsuleHalfHeight;

    // Spawn the actor deferred
    ASf_TP_Character* Enemy = GetWorld()->SpawnActorDeferred<ASf_TP_Character>(
        EnemyClass,
        FTransform(Transform.GetRotation(), AdjustedLocation,FVector(1,1,1)),
        nullptr, // Owner
        nullptr, // Instigator
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);

    if (!Enemy)
    {
        SF_SIMPLE_DEBUG(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Enemy spawn failed!")),
            Spawning::Enemies);
        return;
    }

    // Finalize spawning
    Enemy->FinishSpawning(FTransform(Transform.GetRotation(), AdjustedLocation, FVector(1,1,1)));
	
	RegisterEnemy(Enemy);	
}

void UEnemySpawner::SpawnEnemyRandom()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyTimerHandle);
	
	SpawnEnemy(GetSpawnLocation());
	
	EnemiesToBeSpawned = FMath::Max(EnemiesToBeSpawned - 1, 0);
	if (EnemiesToBeSpawned > 0)
	{
		RequestNewEnemy();
	}
}

void UEnemySpawner::RegisterEnemy(ASf_TP_Character* Enemy)
{
	SF_SIMPLE_DEBUG(LogEnemySpawner, Log, FColor::White, *FString::Printf(TEXT("Register Enemy")), Spawning::Enemies);

	if(Enemy && Enemy->GetFeatureByClass<UCF_Death>())
	{
		Enemy->GetFeatureByClass<UCF_Death>()->OnDeath_CPP.AddLambda([this, Enemy]()
		{
			SF_SIMPLE_DEBUG(
				LogEnemySpawner, 
				Warning, 
				FColor::White, 
				*FString::Printf(TEXT("On Death Triggered")), 
				Spawning::Enemies);
			EnemiesToBeSpawned++;
			UsedEnemyNames.Remove(Enemy->GetName());
			FString ModifiedName = GetIncrementedName(Enemy->GetName());
			PossibleEnemyNames.Add(ModifiedName);
		
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
	
	SF_SIMPLE_DEBUG(LogEnemySpawner, Log, FColor::White, *FString::Printf(TEXT("New Enemy Request process")), Spawning::Enemies);

	if (EnemiesToBeSpawned > 0)
	{
		if (!SpawnEnemyTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimerHandle,this, &UEnemySpawner::SpawnEnemyRandom, SpawnDelay);
	}
}

TSubclassOf<ASf_TP_Character> UEnemySpawner::EvaluateSpawnedEnemyClass() const
{
	return SpawnedEnemyClass;
}

TArray<ASf_PatrolArea*> UEnemySpawner::GetViableSpawns() const
{
	return  USf_PatrolAreaManager::Get(this->GetWorld())->GetFreePatrolAreas();
}

FTransform UEnemySpawner::GetSpawnLocation() const
{
	TArray<ASf_PatrolArea*> PossibleTransforms = GetViableSpawns();
	if (PossibleTransforms.Num() <= 0)
		return FTransform();
	
	//TODO: Currently chooses a random location
	
	int RandomIndex = FMath::RandRange(0, PossibleTransforms.Num() - 1);
	return PossibleTransforms[RandomIndex]->GetRandomMarkerTransform();
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

FString UEnemySpawner::RequestEnemyName()
{
	if (PossibleEnemyNames.Num() <= 0)
		return FString("Tony_" + FString::FromInt(TonyCount++));
	
	int RandomIndex = FMath::RandRange(0, PossibleEnemyNames.Num() - 1);
	FString Name = PossibleEnemyNames[RandomIndex];
	UsedEnemyNames.Add(Name);
	PossibleEnemyNames.Remove(Name);
	return Name;
}

FString UEnemySpawner::GetIncrementedName(FString OriginalName)
{
	FString NewName = OriginalName;
	int32 LastIndex = OriginalName.Len() - 1;
	
	int32 NumberIndex = LastIndex;
	while (NumberIndex >= 0 && FChar::IsDigit(OriginalName[NumberIndex]))
	{
		--NumberIndex;
	}

	if (NumberIndex < LastIndex)
	{
		FString BaseName = OriginalName.Left(NumberIndex + 1);
		FString NumberPart = OriginalName.Mid(NumberIndex + 1);
		int32 Number = FCString::Atoi(*NumberPart);
		NewName = BaseName + FString::Printf(TEXT("%d"), Number + 1);
	}
	else
	{
		NewName = OriginalName + TEXT("_0");
	}

	return FString(NewName);
}