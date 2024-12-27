
#include "CF_Locomotion_Config.h"
#include "CF_Locomotion.h"


TSubclassOf<USf_CharacterFeature> UCF_Locomotion_Config::GetFeatureType_Implementation() const
{
	return UCF_Locomotion::StaticClass();
}

