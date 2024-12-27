#pragma once
#include "CoreMinimal.h"
#include "SF_TP_DynamicMoveTarget.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_DynamicMoveTarget.generated.h"


//DEFINE_LOG_CATEGORY_STATIC(EF_Death, Display, Display);

class UCF_DynamicMoveTarget_Config;

UCLASS(Blueprintable)
class STARFIRE_API UCF_DynamicMoveTarget : public USf_CharacterFeature
{
	GENERATED_BODY()

public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;
	virtual void OnBeginPlay_Implementation() override;
	virtual void OnTick_Implementation(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ASf_TP_DynamicMoveTarget* GetMoveTarget() const;

#pragma region Functions
protected:
	bool TrySetTarget(float DeltaTime);
#pragma endregion
	
#pragma region Properties
public:
protected:
	UPROPERTY(BlueprintReadOnly,Category = "Movement")
	ASf_TP_DynamicMoveTarget* MoveTarget = nullptr;

	UPROPERTY()
	const UCF_DynamicMoveTarget_Config* DynamicMoveTargetConfig;

	UPROPERTY()
	bool bHasReachablePoint;
private:
	float TimeSinceLastUpdate = 0.0f;
#pragma endregion


};