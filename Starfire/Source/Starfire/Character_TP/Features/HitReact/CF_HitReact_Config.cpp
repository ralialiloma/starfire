// Fill out your copyright notice in the Description page of Project Settings.


#include "CF_HitReact_Config.h"

#include "CF_HitReact.h"

TSubclassOf<USf_CharacterFeature> UCF_HitReact_Config::GetFeatureType_Implementation() const
{
	return UCF_HitReact::StaticClass();
}
