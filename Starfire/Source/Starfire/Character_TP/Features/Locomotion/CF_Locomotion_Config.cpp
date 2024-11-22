
#include "CF_Locomotion_Config.h"
#include "CF_Locomotion.h"


TSubclassOf<USf_CharacterFeature> UCF_Locomotion_Config::GetFeatureType() const
{
	return UCF_Locomotion::StaticClass();
}

