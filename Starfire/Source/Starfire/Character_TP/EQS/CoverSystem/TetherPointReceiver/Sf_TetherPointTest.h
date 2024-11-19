#pragma once
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Sf_TetherPointTest.generated.h"


UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew)
class STARFIRE_API USf_TetherPointTest : public UObject
{
	GENERATED_BODY()

#pragma region Functions
public:
	
	UFUNCTION()
	virtual TArray<TWeakObjectPtr<UTetherPoint>> GetTetherPointsBlocking(TWeakObjectPtr<AActor> OwningActor,
	                                                                     TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
	                                                                     TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints);
protected:
	void FinishExecute();
#pragma endregion
	
#pragma region Properties
private:
	bool bFinishedExecute = false;
#pragma endregion


};


