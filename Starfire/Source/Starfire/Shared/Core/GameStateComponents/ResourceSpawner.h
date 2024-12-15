#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ManagerActor.h"
#include "Starfire/Shared/Interact/Interactables/Resource.h"
#include "Starfire/Shared/Interact/Interactables/ResourceSpawnLocation.h"
#include "ResourceSpawner.generated.h"

USTRUCT(BlueprintType)
struct FResourceSpawn
{
	GENERATED_BODY()

public:
#pragma region Constructors
	FResourceSpawn() {  }
	FResourceSpawn(const FTransform& InTransform)
	{
		Transform = InTransform;
	}
	FResourceSpawn(AResource* InItem)
	{
		if (!InItem)
			return;
		
		Transform = InItem->GetTransform();
		ItemTag = InItem->GetItemTag();
		ResourcePtr = InItem;
	}
	explicit FResourceSpawn(const AResourceSpawnLocation* InItem)
	{
		if (!InItem)
			return;
		
		Transform = InItem->GetTransform();
		ItemTag = InItem->GetItemTag();
		ResourcePtr = nullptr;
	}
#pragma endregion

	FGameplayTag GetItemTag() const
	{
		return ItemTag;
	}
	AResource* GetItemPtr() const
	{
		return ResourcePtr;
	}
	FVector GetLocation() const
	{
		return Transform.GetLocation();
	}
	bool IsOccupied() const
	{
		return IsValid(ResourcePtr);
	}
	
	bool SetItemType(FGameplayTag NewType)
	{
		if (IsOccupied())
			return false;

		ItemTag = NewType;
		return true;
	}
	bool SetItem(AResource* NewResource)
	{
		if (IsOccupied())
			return false;

		if (!NewResource->GetItemTag().MatchesTagExact(ItemTag))
			return false;

		ResourcePtr = NewResource;
		NewResource->SetActorTransform(Transform);
		return true;
	}
	void ClearItem(bool Destroy = false)
	{
		if (!IsOccupied())
			return;
		
		if (Destroy)
			ResourcePtr->Destroy();
		
		ResourcePtr = nullptr;
	}

	bool operator==(const FResourceSpawn& Other) const
	{
		return Transform.Equals(Other.Transform) && ItemTag.MatchesTagExact(Other.ItemTag) && ResourcePtr == Other.ResourcePtr;
	}


protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Transform = FTransform();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Gameplay.Resource"))
	FGameplayTag ItemTag;
	UPROPERTY()
	AResource* ResourcePtr = nullptr;
	
};

DECLARE_MULTICAST_DELEGATE_OneParam(FVeinEmpty, uint8 /*VeinID*/)

DECLARE_LOG_CATEGORY_EXTERN(LogResourceSpawner, Log, All);

USTRUCT(BlueprintType)
struct FResourceVein
{
	GENERATED_BODY()

	FResourceVein() {  }
	FResourceVein(uint8 InVeinID, const TArray<TSharedPtr<FResourceSpawn>>& InVeins)
	{
		VeinID = InVeinID;
		Spawns = InVeins;
	}

public:

	FVeinEmpty OnVeinEmpty;

	uint8 GetNumOccupiedSpawns() const;
	uint8 GetVeinID() const;
	
	bool AddResource(AResource* Resource);
	bool AddSpawn(TSharedPtr<FResourceSpawn> Spawn);

	FVector GetVeinCenter() const;

	bool operator==(const FResourceVein& Other) const
	{
		return VeinID == Other.VeinID;
	}

protected:
	
	TArray<int> GetViableSpawnIndexes();

	void OnResourceCollected(AResource* Resource);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 VeinID = 0;
	
	TArray<TSharedPtr<FResourceSpawn>> Spawns {};
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), PrioritizeCategories = "Settings")
class STARFIRE_API AResourceSpawner : public AManagerActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void StartGame() override;
	
	void SpawnResourceVeinRandom(bool QueueNewVein = true);
	void SpawnResourceVeinRelay();

	UFUNCTION()
	void OnVeinEmpty(uint8 VeinID);
	
	void QueueSpawnCooldowns(TSharedPtr<FResourceVein> Spawn);
	void QueueNewResourceVein();

protected:

	UFUNCTION()
	TArray<int> GetViableResourceVeinIndexes() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	TSubclassOf<AResource> ResourceClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	float SpawnDelay = 10;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	int MaxSpawnedVeins = 5;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	int SpawnedPerVein = 3;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	bool StartWithMax = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Settings")
	int NoSpawnCooldown = 2;


	
	TArray<TSharedPtr<FResourceVein>> EmptyResourceVeins {};
	TArray<TSharedPtr<FResourceVein>> OccupiedVeins {};
	
	TArray<TSharedPtr<FResourceVein>> CooldownVeins {};
	UPROPERTY()
	FTimerHandle SpawnVeinTimerHandle;

};