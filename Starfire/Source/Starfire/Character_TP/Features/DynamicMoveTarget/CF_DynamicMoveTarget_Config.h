#pragma once
#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_DynamicMoveTarget_Config.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UCF_DynamicMoveTarget_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType() const override;
	
#pragma region Properties
public:
	UPROPERTY(EditDefaultsOnly)
	float FrameRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MinDistance = 200;

	UPROPERTY(EditDefaultsOnly)
	float MaxDistance = 500;

	UPROPERTY(EditDefaultsOnly)
	FVector ProjectionExtent = FVector( 150,150,150);

	UPROPERTY(EditDefaultsOnly)
	float MaxVerticalTraceLength = 100000.f;;
#pragma endregion


};


