
#include "CF_Combat_Config.h"
#include "CF_Combat.h"


TSubclassOf<USf_CharacterFeature> UCF_Combat_Config::GetFeatureType_Implementation() const
{
	return UCF_Combat::StaticClass();
}

