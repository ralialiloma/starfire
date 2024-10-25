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
	FriendlyFire = 6,
	SensedPlayer = 7
};

UENUM(BlueprintType)
enum class ELocationBlackboardKey : uint8
{
	None = 0 UMETA(Hidden), // Hidden value for none
	LastPlayerLocation = 1,
	CoverLocation = 2,
	PeakLocation = 3,
	RandomRoamLocation = 4,
	CloseToPlayerLoc = 5,
	ClosestEnemyLocation = 6,
};

UCLASS(BlueprintType)
class STARFIRE_API UBlackboardKeyHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// For Float Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey, float Value);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Blackboard|Getters")
	static float GetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Clear")
	static void ClearFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey);

	// For Int Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey, int32 Value);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Blackboard|Getters")
	static int32 GetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Clear")
	static void ClearIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey);

	// For Bool Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey, bool Value);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Blackboard|Getters")
	static bool GetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Clear")
	static void ClearBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey);

	// For Vector Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	static void SetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey, FVector Value);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Blackboard|Getters")
	static FVector GetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Clear")
	static void ClearVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Blackboard|IsSet")
    static bool IsSetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey VectorBlackboardKey);
};
