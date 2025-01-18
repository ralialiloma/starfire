// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_HitReact_Config.generated.h"

/**
 * 
 */
UCLASS()
class STARFIRE_API UCF_HitReact_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType_Implementation() const override;

};
