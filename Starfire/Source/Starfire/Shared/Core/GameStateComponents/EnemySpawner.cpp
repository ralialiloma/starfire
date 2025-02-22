// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Features/Death/CF_Death.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolAreaManager.h"
#include "DebugFunctionLibrary.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogEnemySpawner);

void AEnemySpawner::StartGame()
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

void AEnemySpawner::DelayedStartGame()
{
	TArray<ASf_PatrolArea*> PatrolAreas = USf_PatrolAreaManager::GetAllPatrolAreas(this);
	if (MaxEnemies > PatrolAreas.Num())
	{
		DEBUG_SIMPLE(
			LogEnemySpawner,
			Warning,
			FColor::Orange,
			*FString::Printf(TEXT("MaxEnemies changed from %i to %i"), MaxEnemies, PatrolAreas.Num()),
			Sf_GameplayTags::Debug::Spawning::Enemies);

		MaxEnemies = PatrolAreas.Num();
	}

	// if (bWaitForVeins)
	//     return;

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

void AEnemySpawner::SpawnEnemy(const FTransform& Transform)
{
    DEBUG_SIMPLE(
        LogEnemySpawner,
        Warning,
        FColor::White,
        *FString::Printf(TEXT("Spawning Enemy")),
        Sf_GameplayTags::Debug::Spawning::Enemies);

    UClass* EnemyClass = EvaluateSpawnedEnemyClass();
    if (!EnemyClass)
    {
        DEBUG_SIMPLE(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Spawning Class Invalid!")),
            Sf_GameplayTags::Debug::Spawning::Enemies);
        return;
    }

    FActorSpawnParameters SpawnParameters{};
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Retrieve default object to access capsule size
    const ASf_TP_Character* DefaultEnemy = EnemyClass->GetDefaultObject<ASf_TP_Character>();
    if (!DefaultEnemy)
    {
        DEBUG_SIMPLE(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Failed to retrieve default object for enemy class!")),
            Sf_GameplayTags::Debug::Spawning::Enemies);
        return;
    }

    // Get capsule component half-height
    const UCapsuleComponent* Capsule = DefaultEnemy->GetCapsuleComponent();
    if (!Capsule)
    {
        DEBUG_SIMPLE(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Enemy class does not have a capsule component!")),
            Sf_GameplayTags::Debug::Spawning::Enemies);
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
        DEBUG_SIMPLE(
            LogEnemySpawner,
            Error,
            FColor::Red,
            *FString::Printf(TEXT("Enemy spawn failed!")),
            Sf_GameplayTags::Debug::Spawning::Enemies);
        return;
    }

    // Finalize spawning
    Enemy->FinishSpawning(FTransform(Transform.GetRotation(), AdjustedLocation, FVector(1,1,1)));
	
	RegisterEnemy(Enemy);	
}

void AEnemySpawner::SpawnEnemyRandom()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyTimerHandle);
	
	SpawnEnemy(GetSpawnLocation());
	
	EnemiesToBeSpawned = FMath::Max(EnemiesToBeSpawned - 1, 0);
	if (EnemiesToBeSpawned > 0)
	{
		RequestNewEnemy();
	}
}

void AEnemySpawner::RegisterEnemy(ASf_TP_Character* Enemy)
{
	DEBUG_SIMPLE(LogEnemySpawner, Log, FColor::White, *FString::Printf(TEXT("Register Enemy")), Sf_GameplayTags::Debug::Spawning::Enemies);

	if(Enemy && Enemy->GetFeatureByClass<UCF_Death>())
	{
		Enemy->GetFeatureByClass<UCF_Death>()->OnDeath_CPP.AddLambda([this, Enemy]()
		{
			DEBUG_SIMPLE(
				LogEnemySpawner, 
				Warning, 
				FColor::White, 
				*FString::Printf(TEXT("On Death Triggered")), 
				Sf_GameplayTags::Debug::Spawning::Enemies);
			EnemiesToBeSpawned++;
			UsedEnemyNames.Remove(Enemy->GetName());
			FString ModifiedName = GetIncrementedName(Enemy->GetName());
			PossibleEnemyNames.Add(ModifiedName);
		
			RequestNewEnemy();
		});
	}
}

void AEnemySpawner::RequestNewEnemy()
{
	if (EnemiesToBeSpawned <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnEnemyTimerHandle);
		return;
	}
	
	DEBUG_SIMPLE(LogEnemySpawner, Log, FColor::White, *FString::Printf(TEXT("New Enemy Request process")), Sf_GameplayTags::Debug::Spawning::Enemies);

	if (EnemiesToBeSpawned > 0)
	{
		if (!SpawnEnemyTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnEnemyTimerHandle,this, &AEnemySpawner::SpawnEnemyRandom, SpawnDelay);
	}
}

TSubclassOf<ASf_TP_Character> AEnemySpawner::EvaluateSpawnedEnemyClass() const
{
	return SpawnedEnemyClass;
}

TArray<ASf_PatrolArea*> AEnemySpawner::GetViableSpawns() const
{
	if (USf_PatrolAreaManager::Get(GetWorld()))
	{
		TArray<ASf_PatrolArea*> FreePatrolAreas = USf_PatrolAreaManager::Get(this->GetWorld())->GetFreePatrolAreas();
		TArray<ASf_PatrolArea*> ViableSpawns {};

		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (!Character)
			return FreePatrolAreas;
		
		FVector PlayerLocation = Character->GetActorLocation();

		for (auto FreePatrolArea : FreePatrolAreas)
		{
			if (FreePatrolArea)
			{
				FVector PatrolAreaLocation = FreePatrolArea->GetActorLocation();
				
				FVector ToPatrolArea = PatrolAreaLocation - PlayerLocation;
				float DistanceToPatrolArea = ToPatrolArea.Size();
				
				if (DistanceToPatrolArea >= MinSpawnDistance)
					ViableSpawns.Add(FreePatrolArea);						
			}
		}

		for (auto ViableSpawn : ViableSpawns)
		{
			UKismetSystemLibrary::DrawDebugBox(this, ViableSpawn->GetActorLocation(), FVector::One() * 100, FColor::Red);
		}

		return ViableSpawns;
	}
	return {};
}

FTransform AEnemySpawner::GetSpawnLocation() const
{
	TArray<ASf_PatrolArea*> PossibleTransforms = GetViableSpawns();
	if (PossibleTransforms.Num() <= 0)
		return FTransform();
	
	//TODO: Currently chooses a random location
	
	int RandomIndex = FMath::RandRange(0, PossibleTransforms.Num() - 1);
	if (PossibleTransforms[RandomIndex])
		return PossibleTransforms[RandomIndex]->GetRandomMarkerTransform();

	return FTransform();
}

TArray<ASf_TP_Character*> AEnemySpawner::GetAllEnemies() const
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

FString AEnemySpawner::RequestEnemyName()
{
	if (PossibleEnemyNames.Num() <= 0)
		return FString("Tony_" + FString::FromInt(TonyCount++));
	
	int RandomIndex = FMath::RandRange(0, PossibleEnemyNames.Num() - 1);
	FString Name = PossibleEnemyNames[RandomIndex];
	UsedEnemyNames.Add(Name);
	PossibleEnemyNames.Remove(Name);
	return Name;
}

FString AEnemySpawner::GetIncrementedName(FString OriginalName)
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