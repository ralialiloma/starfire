#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "EF_Locomotion.generated.h"





#pragma region Enums And Structs

UENUM(BlueprintType)
enum class E_Sf_TP_LocomotionType: uint8
{
	KeepCurrent = 0,
	Walk = 2,
	Sprint = 3,
	Crouch = 4,
};

USTRUCT(BlueprintType)
struct F_SF_MoveRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Destination;
	UPROPERTY(BlueprintReadWrite)
	float AcceptanceRadius;
	UPROPERTY(BlueprintReadWrite)
	E_Sf_TP_LocomotionType LocomotionType;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	F_SF_MoveRequest()
		: Destination(FVector::ZeroVector)
		, AcceptanceRadius(100.f)
		, LocomotionType(E_Sf_TP_LocomotionType::Walk)
		, FilterClass(nullptr)
	{
	}
};

#pragma endregion

DEFINE_LOG_CATEGORY_STATIC(EF_Locomotion, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UEF_Locomotion : public UEnemyFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Locomotion")
	bool MoveToLocation(F_SF_MoveRequest MoveRequest);
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Locomotion")
	void StopMovement();
private:
	void ProcessLocomotionType(E_Sf_TP_LocomotionType LocomotionType);
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID,EPathFollowingResult::Type Result);
	void ClearAllDelegates();
#pragma endregion
	
#pragma region Properties
public:
	FSf_VoidDelegate_CPP OnMoveFinished_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveFinished_BP;

	FSf_VoidDelegate_CPP OnMoveFailed_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveFailed_BP;
private:
	UPROPERTY()
	FVector LastDestination;
#pragma endregion


};