#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "EF_Locomotion.generated.h"





#pragma region Enums And Structs

UENUM(BlueprintType)
enum class E_NP_LocomotionType: uint8
{
	KeepCurrent = 0,
	Walk = 2,
	Sprint = 3,
	Crouch = 4,
};

USTRUCT(BlueprintType)
struct FMoveRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Destination;
	UPROPERTY(BlueprintReadWrite)
	float AcceptanceRadius;
	UPROPERTY(BlueprintReadWrite)
	bool AvoidPlayerSight;
	UPROPERTY(BlueprintReadWrite)
	E_NP_LocomotionType LocomotionType;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	FMoveRequest()
		: Destination(FVector::ZeroVector)
		, AcceptanceRadius(0.0f)
		, AvoidPlayerSight(false)
		, LocomotionType(E_NP_LocomotionType::Walk)
		, FilterClass(nullptr)
	{
	}
};

#pragma endregion

UCLASS(Blueprintable)
class STARFIRE_API UEF_Locomotion : public UEnemyFeature
{
	GENERATED_BODY()

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(FMoveRequest MoveRequest);
	UFUNCTION(BlueprintCallable)
	void StopMoving();
private:
	void ProcessLocomotionType(E_NP_LocomotionType LocomotionType);
#pragma endregion
	
#pragma region Properties
public:
	FSf_VoidDelegate_CPP OnMoveFinished_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveFinished_BP;
private:
	UPROPERTY()
	FVector LastDestination;
#pragma endregion


};