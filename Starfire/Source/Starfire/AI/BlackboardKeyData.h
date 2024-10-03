#pragma once
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BlackboardKeyData.generated.h"

USTRUCT(BlueprintType)
struct FBlackboardKeyData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector LastPlayerLocation;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector CoverLocation;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector PeakLocation;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector RandomRoamLocation;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector CloseToPlayerLoc;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector RemainingHealth;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector IsUnderFire;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector ThreatLevel;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector ElapsedTimeSinceView;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector IsInCover;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector FriendlyFire;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector CanMelee;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector HasToReload;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector SelfActor;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector ClosestEnemyLocation;

	UPROPERTY(BlueprintReadOnly)
	FBlackboardKeySelector IsDead;

	FBlackboardKeyData()
	{
		LastPlayerLocation.SelectedKeyName = "LastPlayerLocation";
		CoverLocation.SelectedKeyName = "CoverLocation";
		PeakLocation.SelectedKeyName = "PeakLocation";
		RandomRoamLocation.SelectedKeyName = "RandomRoamLocation";
		CloseToPlayerLoc.SelectedKeyName = "CloseToPlayerLoc";
		RemainingHealth.SelectedKeyName = "RemainingHealth";
		IsUnderFire.SelectedKeyName = "IsUnderFire";
		ThreatLevel.SelectedKeyName = "ThreatLevel";
		ElapsedTimeSinceView.SelectedKeyName = "ElapsedTimeSinceView";
		IsInCover.SelectedKeyName = "IsInCover";
		FriendlyFire.SelectedKeyName = "FriendlyFire";
		CanMelee.SelectedKeyName = "CanMelee";
		HasToReload.SelectedKeyName = "HasToReload";
		SelfActor.SelectedKeyName = "SelfActor";
		ClosestEnemyLocation.SelectedKeyName = "ClosestEnemyLocation";
		IsDead.SelectedKeyName = "IsDead";
	}

	static const FBlackboardKeyData& Get()
	{
		static FBlackboardKeyData Instance;
		return Instance;
	}
	
};
