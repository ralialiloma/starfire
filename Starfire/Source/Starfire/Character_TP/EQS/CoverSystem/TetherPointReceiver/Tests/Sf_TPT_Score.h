#pragma once
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Character_TP/EQS/CoverSystem/TetherPointReceiver/Sf_TetherPointTest.h"
#include "Sf_TPT_Score.generated.h"

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew)
class STARFIRE_API USf_TPT_Score : public USf_TetherPointTest
{
	GENERATED_BODY()

#pragma region Functions
public:
	virtual TArray<TWeakObjectPtr<UTetherPoint>> GetTetherPointsBlocking(TWeakObjectPtr<AActor>,
	                                                                     TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
	                                                                     TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints) override;
#pragma endregion
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere)
	float MinScore = 0;

	UPROPERTY(EditAnywhere)
	float MaxScore = 1;
#pragma endregion
};