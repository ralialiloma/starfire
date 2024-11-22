#pragma once
#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_Cover_Config.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UCF_Cover_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType() const override;
	
#pragma region Properties
public:
	UPROPERTY(EditDefaultsOnly)
	float MinCoverHeight = 120;

	UPROPERTY(EditDefaultsOnly)
	float MaxCrouchCoverHeight = 60;
#pragma endregion


};


