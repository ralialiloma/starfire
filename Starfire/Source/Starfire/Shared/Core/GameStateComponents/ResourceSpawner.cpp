#include "ResourceSpawner.h"

#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/Interact/Interactables/ResourceSpawnLocation.h"

uint8 FResourceVein::GetNumOccupiedSpawns() const
{
	int NumOccupiedSpawns = 0;
	for (auto ResourceSpawn : Spawns)
	{
		if (ResourceSpawn.IsOccupied())
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
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Resouces Collected");

	
	FResourceSpawn* Spawn = Spawns.FindByPredicate([Resource](const FResourceSpawn& Spawn)
	{
		return Resource == Spawn.GetItemPtr();
	});

	check(Spawn);
	Spawn->ClearItem();

	if (GetNumOccupiedSpawns() <= 0)
		OnVeinEmpty.Broadcast(VeinID);
}

bool FResourceVein::AddResource(AResource* Resource)
{
	TArray<int> ViableSpawns = GetViableSpawnIndexes();
	if (ViableSpawns.Num() <= 0)
		return false;

	int RandomSpawnIndex = FMath::RandRange(0, ViableSpawns.Num() - 1);
	FResourceSpawn& Spawn = Spawns[ViableSpawns[RandomSpawnIndex]];
	Spawn.SetItem(Resource);
	Resource->OnCollectDelegate_CPP.AddLambda([this](AResource* Resource)
	{
		OnResourceCollected(Resource);
	});

	return true;
}

bool FResourceVein::AddSpawn(FResourceSpawn Spawn)
{
	Spawns.Add(Spawn);
	return true;
}

TArray<int> FResourceVein::GetViableSpawnIndexes()
{
	if (Spawns.Num() <= 0)
		return TArray<int>();
	
	TArray<int> ReturnSpawns {};
	for (int i = 0; i < Spawns.Num(); ++i)
	{
		if (Spawns[i].IsOccupied())
			continue;

		ReturnSpawns.Add(i);
	}
	return ReturnSpawns;
}

void UResourceSpawner::StartGame()
{
	Super::StartGame();

	if (!ResourceClass)
	{
		DestroyComponent();
		return;
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, AResourceSpawnLocation::StaticClass(), OutActors);

	for (AActor* OutActor : OutActors)
	{
		AResourceSpawnLocation* ResourceSpawnLocation = Cast<AResourceSpawnLocation>(OutActor);
		if (!ResourceSpawnLocation)
			continue;

		auto Vein = EmptyResourceVeins.FindByPredicate([ResourceSpawnLocation](const FResourceVein& Spawn)
		{
			return ResourceSpawnLocation->GetVeinGroup() == Spawn.GetVeinID();
		});
		
		if (Vein)
			Vein->AddSpawn(ResourceSpawnLocation);
		else
		{
			FResourceVein NewVein = FResourceVein(ResourceSpawnLocation->GetVeinGroup(), { ResourceSpawnLocation });
			int Index = EmptyResourceVeins.Add(NewVein);
			EmptyResourceVeins[Index].OnVeinEmpty.AddUObject(this, &UResourceSpawner::OnVeinEmpty);
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

void UResourceSpawner::SpawnResourceVeinRandom(bool QueueNewVein)
{
	if (OccupiedVeins.Num() >= MaxSpawnedVeins)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnVeinTimerHandle);

	TArray<int> ViableSpawns = GetViableResourceVeinIndexes();
	int Index = ViableSpawns[FMath::RandRange(0,ViableSpawns.Num() - 1)];
	check(Index != INDEX_NONE)
	
	FResourceVein& Spawn = EmptyResourceVeins[Index];
	EmptyResourceVeins.RemoveAt(Index);

	for (int i = 0; i < SpawnedPerVein; ++i)
	{
		AResource* Resource = GetWorld()->SpawnActor<AResource>(ResourceClass, FTransform());
		if (!Spawn.AddResource(Resource))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Somthing Went Wrong Resource Spawner");
	}
	
	OccupiedVeins.Add(Spawn);

	if (QueueNewVein && OccupiedVeins.Num() < MaxSpawnedVeins)
		QueueNewResourceVein();
}

void UResourceSpawner::SpawnResourceVeinRelay()
{
	SpawnResourceVeinRandom();
}

void UResourceSpawner::OnVeinEmpty(uint8 VeinID)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::FromInt(VeinID) + " Vein is empty");

	check(VeinID);
	FResourceVein* Spawn = OccupiedVeins.FindByPredicate([VeinID](const FResourceVein& Spawn)
	{
		return VeinID == Spawn.GetVeinID();
	});
	check(Spawn);

	FResourceVein& ResourceVein = *Spawn;
	OccupiedVeins.Remove(ResourceVein);
	EmptyResourceVeins.Add(ResourceVein);
	QueueSpawnCooldowns(ResourceVein);
	QueueNewResourceVein();
}

void UResourceSpawner::QueueSpawnCooldowns(const FResourceVein& Spawn)
{
	CooldownVeins.Add(Spawn);
	if (CooldownVeins.Num() >= NoSpawnCooldown)
		CooldownVeins.RemoveAt(0);
}

void UResourceSpawner::QueueNewResourceVein()
{
	if (SpawnDelay <= 0)
	{
		SpawnResourceVeinRandom();
	}
	else
	{
		if (!SpawnVeinTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnVeinTimerHandle, this, &UResourceSpawner::SpawnResourceVeinRelay, SpawnDelay);
	}
}

TArray<int> UResourceSpawner::GetViableResourceVeinIndexes() const
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
