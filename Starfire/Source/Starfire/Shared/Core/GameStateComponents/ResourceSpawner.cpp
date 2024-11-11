#include "ResourceSpawner.h"

#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/Interact/Interactables/ResourceSpawnLocation.h"

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
		AResourceSpawnLocation* Resource = Cast<AResourceSpawnLocation>(OutActor);
		if (!Resource)
			continue;
		
		EmptyResourceTransforms.Add(Resource);
		Resource->Destroy();
	}
	Algo::RandomShuffle(EmptyResourceTransforms);

	if (StartWithMax)
	{
		for (int i = 0; i < MaxSpawned; ++i)
		{
			SpawnResourceRandom();
		}
	}
	else
	{
		SpawnResourceRandom();
	}
}

void UResourceSpawner::SpawnResourceRandom()
{
	if (OccupiedTransforms.Num() >= MaxSpawned)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnItemTimerHandle);
	AResource* Resource = GetWorld()->SpawnActor<AResource>(ResourceClass, FTransform());

	TArray<FResourceSpawn> ViableSpawns = GetViableResourceTransforms();
	FResourceSpawn RandomSpawn = ViableSpawns[FMath::RandRange(0,ViableSpawns.Num() - 1)];
	int FoundIndex = ViableSpawns.Find(RandomSpawn);
	check(FoundIndex != INDEX_NONE)
	
	FResourceSpawn Spawn = EmptyResourceTransforms[FoundIndex];
	EmptyResourceTransforms.RemoveAt(FoundIndex);
	
	Spawn.SetItem(Resource);
	OccupiedTransforms.Add(Spawn);
	Resource->OnCollectDelegate.AddDynamic(this, &UResourceSpawner::OnTrackedItemCollected);

	if (OccupiedTransforms.Num() < MaxSpawned)
	{
		QueueNewResource();
	}
}

void UResourceSpawner::OnTrackedItemCollected(AResource* Item)
{
	check(Item);
	FResourceSpawn* Spawn = OccupiedTransforms.FindByPredicate([Item](const FResourceSpawn& Spawn)
	{
		return Item == Spawn.GetItemPtr();
	});
	check(Spawn);

	FResourceSpawn SpawnLoc = *Spawn;
	OccupiedTransforms.Remove(SpawnLoc);
	SpawnLoc.ClearItem();
	EmptyResourceTransforms.Add(SpawnLoc);
	QueueSpawnCooldowns(SpawnLoc);
	QueueNewResource();
}

void UResourceSpawner::QueueSpawnCooldowns(const FResourceSpawn& Spawn)
{
	CooldownTransforms.Add(Spawn);
	if (CooldownTransforms.Num() >= NoSpawnCooldown)
		CooldownTransforms.RemoveAt(0);
}

void UResourceSpawner::QueueNewResource()
{
	if (SpawnDelay <= 0)
	{
		SpawnResourceRandom();
	}
	else
	{
		if (!SpawnItemTimerHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(SpawnItemTimerHandle, this, &UResourceSpawner::SpawnResourceRandom, SpawnDelay);
	}
}

TArray<FResourceSpawn> UResourceSpawner::GetViableResourceTransforms() const
{
	TArray<FResourceSpawn> ReturnArray {};

	for (auto Element : EmptyResourceTransforms)
	{
		if (!CooldownTransforms.Contains(Element))
		{
			ReturnArray.Add(Element);
		}
	}
	return ReturnArray;
}
