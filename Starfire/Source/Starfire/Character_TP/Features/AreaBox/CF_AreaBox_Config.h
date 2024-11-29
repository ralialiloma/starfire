#pragma once
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_AreaBox_Config.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UCF_AreaBox_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType() const override;
	
#pragma region Properties
#pragma endregion


};


