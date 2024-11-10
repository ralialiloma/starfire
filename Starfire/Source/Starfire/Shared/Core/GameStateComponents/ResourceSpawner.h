#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ManagerComponent.h"
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
	FResourceSpawn(const AResourceSpawnLocation* InItem)
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
	bool IsOccupied() const
	{
		return ResourcePtr != nullptr;
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

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class STARFIRE_API UResourceSpawner : public UManagerComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartGame() override;
	
	void SpawnResourceRandom();

	UFUNCTION()
	void OnTrackedItemCollected(AResource* Item);
	
	void QueueSpawnCooldowns(const FResourceSpawn& Spawn);
	void QueueNewResource();

protected:

	UFUNCTION()
	TArray<FResourceSpawn> GetViableResourceTransforms() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AResource> ResourceClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SpawnDelay = 10;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int MaxSpawned = 3;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool StartWithMax = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int NoSpawnCooldown = 2;



	UPROPERTY()
	TArray<FResourceSpawn> EmptyResourceTransforms {};
	UPROPERTY()
	TArray<FResourceSpawn> OccupiedTransforms {};

	UPROPERTY()
	TArray<FResourceSpawn> CooldownTransforms {};
	UPROPERTY()
	FTimerHandle SpawnItemTimerHandle;

};