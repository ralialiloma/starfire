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
	virtual TArray<UTetherPoint*> GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint, TArray<UTetherPoint*> TetherPoints);

private:
#pragma endregion
	
#pragma region Properties
public:

#pragma endregion


};


