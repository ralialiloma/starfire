﻿
#include "CF_EQSData_Config.h"
#include "CF_EQSData.h"


TSubclassOf<USf_CharacterFeature> UCF_EQSData_Config::GetFeatureType() const
{
	return UCF_EQS_Data::StaticClass();
}
