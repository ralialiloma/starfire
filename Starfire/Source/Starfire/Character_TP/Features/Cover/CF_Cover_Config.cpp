
#include "CF_Cover_Config.h"
#include "CF_Cover.h"


TSubclassOf<USf_CharacterFeature> UCF_Cover_Config::GetFeatureType_Implementation() const
{
	return UCF_Cover::StaticClass();
}

