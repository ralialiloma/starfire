﻿
#include "CF_Death_Config.h"
#include "CF_Death.h"


TSubclassOf<USf_CharacterFeature> UCF_Death_Config::GetFeatureType() const
{
	return UCF_Death::StaticClass();
}

