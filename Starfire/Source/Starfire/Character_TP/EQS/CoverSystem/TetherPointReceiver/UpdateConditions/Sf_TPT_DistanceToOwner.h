#pragma once
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Character_TP/EQS/CoverSystem/TetherPointReceiver/Sf_TetherPointTest.h"
#include "Sf_TPT_DistanceToOwner.generated.h"

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew)
class STARFIRE_API USf_TPT_DistanceToOwner : public USf_TetherPointTest
{
	GENERATED_BODY()

#pragma region Functions
public:
	virtual TArray<UTetherPoint*> GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint, TArray<UTetherPoint*> TetherPoints) override;

private:
#pragma endregion
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere)
	float MinDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 1000.f;
#pragma endregion


};