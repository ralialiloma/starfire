#pragma once
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Character_TP/EQS/CoverSystem/TetherPointReceiver/Sf_TetherPointTest.h"
#include "Sf_TPT_ExcludeReservedLocations.generated.h"

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew)
class STARFIRE_API USf_TPT_ExcludeReservedLocations : public USf_TetherPointTest
{
	GENERATED_BODY()

#pragma region Functions
public:

	virtual TArray<UTetherPoint*> GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint, TArray<UTetherPoint*> TetherPoints) override;
protected:
	bool IsLocationReservedTarget(const FVector& CoverLocation) const;
#pragma endregion
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere)
	float RadiusToCheck = 200.f;
#pragma endregion


};