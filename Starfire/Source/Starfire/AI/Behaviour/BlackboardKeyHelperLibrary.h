#pragma once
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BlackboardKeyHelperLibrary.generated.h"

DECLARE_LOG_CATEGORY_CLASS(SF_BlackboardKeyHelper,Log, Log);

// Enum for Float Blackboard Keys
UENUM(BlueprintType)
enum class EFloatBlackboardKey : uint8
{
	None = 0 UMETA(Hidden), // Hidden value for none
	RemainingHealth = 1,
	ThreatLevel = 2,
	ElapsedTimeSinceView = 3,
};

// Enum for Int Blackboard Keys
UENUM(BlueprintType)
enum class EIntBlackboardKey : uint8
{
	None = 0 UMETA(Hidden), // Hidden value for none
	IsUnderFire = 1,
	FriendlyFire = 2,
};

// Enum for Bool Blackboard Keys
UENUM(BlueprintType)
enum class EBoolBlackboardKey : uint8
{
	None = 0 UMETA(Hidden), // Hidden value for none
	IsInCover = 1,
	CanMelee = 2,
	HasToReload = 3,
	IsDead = 4,
	IsUnderFire = 5,
	FriendlyFire = 6
};

UENUM(BlueprintType)
enum class EActorBlackboardKey : uint8
{
	None = 0 UMETA(Hidden), // Hidden value for none
	LastPlayerLocation = 1,
	CoverLocation = 2,
	PeakLocation = 3,
	RandomRoamLocation = 4,
	CloseToPlayerLoc = 5,
	SelfActor = 6,
	ClosestEnemyLocation = 7,
};

UCLASS(BlueprintType)
class STARFIRE_API UBlackboardKeyHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// For Float Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey, float Value);

	// For Int Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey, int32 Value);

	// For Bool Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey, bool Value);

	// For Actor Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetActorValue(UBlackboardComponent* BlackboardComp, EActorBlackboardKey ActorBlackboardKey, AActor* Value);
};
