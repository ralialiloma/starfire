﻿#pragma once
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_EQSData_Config.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UCF_EQSData_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType_Implementation() const override;
	
#pragma region Properties
#pragma endregion


};


