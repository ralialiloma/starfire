﻿#include "ResourceSpawner.h"

#include "DebugFunctionLibrary.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogResourceSpawner)

uint8 FResourceVein::GetNumOccupiedSpawns() const
{
	int NumOccupiedSpawns = 0;
	for (auto ResourceSpawn : Spawns)
	{
		if (ResourceSpawn->IsOccupied())
			NumOccupiedSpawns++;
	}
	return NumOccupiedSpawns;
}

uint8 FResourceVein::GetVeinID() const
{
	return VeinID;
}

void FResourceVein::OnResourceCollected(AResource* Resource)
{
	TSharedPtr<FResourceSpawn>* FoundSpawn = nullptr;
	FoundSpawn = Spawns.FindByPredicate([Resource](TSharedPtr<FResourceSpawn> Spawn)
	{
		return Resource == Spawn->GetItemPtr();
	});

	check(FoundSpawn);
	(*FoundSpawn)->ClearItem();

	if (GetNumOccupiedSpawns() <= 0)
		OnVeinEmpty.Broadcast(VeinID);
}

bool FResourceVein::AddResource(AResource* Resource)
{
	TArray<int> ViableSpawns = GetViableSpawnIndexes();
	if (ViableSpawns.Num() <= 0)
		return false;

	int RandomSpawnIndex = FMath::RandRange(0, ViableSpawns.Num() - 1);
	TSharedPtr<FResourceSpawn> Spawn = Spawns[ViableSpawns[RandomSpawnIndex]];

	checkf(Spawn.IsValid(),TEXT( "Spawn is invalid when adding resource"));

#if WITH_EDITOR
	if (!Spawn.IsValid())
		return false;
#endif
	
		
	Spawn->SetItem(Resource);
	Resource->OnCollectDelegate_CPP.AddLambda([this](AResource* Resource)
	{
		OnResourceCollected(Resource);
	});

	return true;
}

bool FResourceVein::AddSpawn(TSharedPtr<FResourceSpawn> Spawn)
{
	Spawns.Add(Spawn);
	return true;
}

FVector FResourceVein::GetVeinCenter() const
{
	if (Spawns.Num() == 0)
		return FVector();

	FVector SumLocation = FVector::ZeroVector;
	
	for (const auto& Spawn : Spawns)
	{
		SumLocation += Spawn->GetLocation();
	}
	
	FVector CenterLocation = SumLocation / Spawns.Num();
	return CenterLocation;
}

TArray<int> FResourceVein::GetViableSpawnIndexes()
{
	if (Spawns.Num() <= 0)
		return TArray<int>();
	
	TArray<int> ReturnSpawns {};
	for (int i = 0; i < Spawns.Num(); ++i)
	{
		if (Spawns[i]->IsOccupied())
			continue;

		ReturnSpawns.Add(i);
	}
	return ReturnSpawns;
}

void AResourceSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceSpawner::StartGame()
{
	Super::StartGame();

	if (!IsValid(ResourceClass))
	{
		Destroy();
		DEBUG_SIMPLE(
			LogResourceSpawner,
			Warning,
			FColor::Yellow,
			*FString::Printf(TEXT("No Resource Class Defined!")),
			Sf_GameplayTags::Debug::Spawning::Resources)
		return;
	}

	TArray<AActor*> OutActors {};
	UGameplayStatics::GetAllActorsOfClass(this, AResourceSpawnLocation::StaticClass(), OutActors);

	for (AActor* OutActor : OutActors)
	{
		AResourceSpawnLocation* ResourceSpawnLocation = Cast<AResourceSpawnLocation>(OutActor);
		if (!ResourceSpawnLocation)
			continue;

		auto Vein = EmptyResourceVeins.FindByPredicate([ResourceSpawnLocation](TSharedPtr<FResourceVein> Spawn)
		{
			return ResourceSpawnLocation->GetVeinGroup() == Spawn->GetVeinID();
		});
		
		if (Vein)
			Vein->Get()->AddSpawn(MakeShared<FResourceSpawn>(FResourceSpawn(ResourceSpawnLocation)));
		else
		{
			TSharedPtr<FResourceVein> NewVein = MakeShared<FResourceVein>(FResourceVein(ResourceSpawnLocation->GetVeinGroup(), { MakeShared<FResourceSpawn>(FResourceSpawn(ResourceSpawnLocation)) }));
			checkf(NewVein, TEXT("Invalid NewVein"));
			int Index = EmptyResourceVeins.Add(NewVein);
			EmptyResourceVeins[Index]->OnVeinEmpty.AddUObject(this, &AResourceSpawner::OnVeinEmpty);
		}
		
		ResourceSpawnLocation->Destroy();
	}
	Algo::RandomShuffle(EmptyResourceVeins);

	if (StartWithMax)
	{
		for (int i = 0; i < MaxSpawnedVeins; ++i)
		{
			SpawnResourceVeinRandom(false);
		}
	}
	else
	{
		SpawnResourceVeinRandom();
	}
}

void AResourceSpawner::SpawnResourceVeinRandom(bool QueueNewVein)
{
	if (OccupiedVeins.Num() >= MaxSpawnedVeins)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnVeinTimerHandle);

	TArray<int> ViableSpawns = GetViableResourceVeinIndexes();
	if (ViableSpawns.Num() <= 0)
		return;
	
	int Index = ViableSpawns[FMath::RandRange(0,ViableSpawns.Num() - 1)];
	check(Index != INDEX_NONE)
	
	TSharedPtr<FResourceVein> Spawn = EmptyResourceVeins[Index];
	EmptyResourceVeins.RemoveAt(Index);

	for (int i = 0; i < SpawnedPerVein; ++i)
	{
		AResource* Resource = GetWorld()->SpawnActor<AResource>(ResourceClass, FTransform());
		if (!Spawn->AddResource(Resource))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Something Went Wrong Resource Spawner");
	}
	
	OccupiedVeins.Add(Spawn);

	if (QueueNewVein && OccupiedVeins.Num() < MaxSpawnedVeins)
		QueueNewResourceVein();
}

void AResourceSpawner::SpawnResourceVeinRelay()
{
	SpawnResourceVeinRandom();
}

void AResourceSpawner::OnVeinEmpty(uint8 VeinID)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::FromInt(VeinID) + " Vein is empty");
	
	TSharedPtr<FResourceVein>* Spawn = OccupiedVeins.FindByPredicate([VeinID](TSharedPtr<FResourceVein> Spawn)
	{
		return VeinID == Spawn->GetVeinID();
	});
	checkf(Spawn!=nullptr,TEXT("Invalid Spawn in OnVeinEmpty"));
	TSharedPtr<FResourceVein> ResourceVein = *Spawn;
	checkf(ResourceVein.IsValid(),TEXT("Invalid Spawn OnVeinEmpty"));
	OccupiedVeins.Remove(ResourceVein);
	EmptyResourceVeins.Add(ResourceVein);
	QueueSpawnCooldowns(ResourceVein);
	QueueNewResourceVein();
}

void AResourceSpawner::QueueSpawnCooldowns(TSharedPtr<FResourceVein> Spawn)
{
	CooldownVeins.Add(Spawn);
	if (CooldownVeins.Num() >= NoSpawnCooldown || CooldownVeins.Num() >= EmptyResourceVeins.Num())
		CooldownVeins.RemoveAt(0);
}

void AResourceSpawner::QueueNewResourceVein()
{
	if (SpawnDelay <= 0)
	{
		SpawnResourceVeinRandom();
	}
	else
	{
		if (!SpawnVeinTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnVeinTimerHandle, this, &AResourceSpawner::SpawnResourceVeinRelay, SpawnDelay);
	}
}

TArray<int> AResourceSpawner::GetViableResourceVeinIndexes() const
{
	TArray<int> ReturnArray {};

	for (int i = 0; i < EmptyResourceVeins.Num(); ++i)
	{
		if (!CooldownVeins.Contains(EmptyResourceVeins[i]))
		{
			ReturnArray.Add(i);
		}
	}
	
	return ReturnArray;
}
