#pragma once
#include "CoreMinimal.h"
#include "CF_Locomotion_Config.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_Locomotion.generated.h"





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
	float ProjectionRadius;
	UPROPERTY(BlueprintReadWrite)
	E_Sf_TP_LocomotionType LocomotionType;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UNavigationQueryFilter> FilterClass;
	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;
	UPROPERTY(BlueprintReadWrite)
	bool bMoveToActor;

	F_SF_MoveRequest()
		: Destination(FVector::ZeroVector)
		  , AcceptanceRadius(100.f)
		  , ProjectionRadius(0), LocomotionType(E_Sf_TP_LocomotionType::Walk)
		  , FilterClass(nullptr)
			,TargetActor(nullptr)
			,	bMoveToActor(false)
	{
	}
};

#pragma endregion

DEFINE_LOG_CATEGORY_STATIC(EF_Locomotion, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UCF_Locomotion : public USf_CharacterFeature
{
	GENERATED_BODY()
public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Locomotion")
	bool MoveToLocation(F_SF_MoveRequest MoveRequest);
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Locomotion")
	void StopMovement();
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Locomotion")
	void FinishMovement();
private:
	void ProcessLocomotionType(E_Sf_TP_LocomotionType LocomotionType);
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID,EPathFollowingResult::Type Result);
	void ClearMoveFailAndFinishDelegates();
#pragma endregion
	
#pragma region Properties
public:
	FSf_VoidDelegate_CPP OnMoveStarted_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveStarted_BP;
	
	FSf_VoidDelegate_CPP OnMoveFinished_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveFinished_BP;

	FSf_VoidDelegate_CPP OnMoveFailed_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnMoveFailed_BP;
protected:
	UPROPERTY()
	const UCF_Locomotion_Config* LocomotionConfig;
	
	UPROPERTY()
	FVector LastDestination;
#pragma endregion


};