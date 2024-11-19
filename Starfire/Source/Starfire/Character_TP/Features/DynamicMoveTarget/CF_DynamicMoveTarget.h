#pragma once
#include "CoreMinimal.h"
#include "SF_TP_DynamicMoveTarget.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Sf_Bases/Sf_CharacterFeature.h"
#include "CF_DynamicMoveTarget.generated.h"


//DEFINE_LOG_CATEGORY_STATIC(EF_Death, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UCF_DynamicMoveTarget : public USf_CharacterFeature
{
	GENERATED_BODY()

public:
	virtual void OnBeginPlay() override;
	virtual void OnTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ASf_TP_DynamicMoveTarget* GetMoveTarget() const;

#pragma region Functions

#pragma endregion
	
#pragma region Properties
public:
protected:
	UPROPERTY(BlueprintReadOnly,Category = "Movement")
	ASf_TP_DynamicMoveTarget* MoveTarget = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float FrameRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MinDistance;

	UPROPERTY(EditDefaultsOnly)
	float MaxDistance;
private:
	float TimeSinceLastUpdate = 0.0f;
#pragma endregion


};