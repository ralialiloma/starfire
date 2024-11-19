#pragma once
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Character_TP/EQS/CoverSystem/TetherPointReceiver/Sf_TetherPointTest.h"
#include "Sf_TPT_DistanceToOwner.generated.h"

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew)
class STARFIRE_API USf_TPT_DistanceToOwner : public USf_TetherPointTest
{
	GENERATED_BODY()

#pragma region Functions
public:
	virtual TArray<TWeakObjectPtr<UTetherPoint>> GetTetherPointsBlocking(TWeakObjectPtr<AActor>,
	                                                                     TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
	                                                                     TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints) override;
protected:
	void OnNavPathQueryFinish(uint32,ENavigationQueryResult::Type,FNavPathSharedPtr);


private:
#pragma endregion
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere)
	float MinDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 1000.f;
protected:
	bool bFinishedNavQuery = false;
	FNavPathSharedPtr NavPathShared;
#pragma endregion


};