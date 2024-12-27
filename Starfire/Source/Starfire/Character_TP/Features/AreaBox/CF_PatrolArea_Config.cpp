
#include "CF_PatrolArea_Config.h"
#include "CF_PatrolArea.h"


TSubclassOf<USf_CharacterFeature> UCF_PatrolArea_Config::GetFeatureType_Implementation() const
{
	return UCF_PatrolArea::StaticClass();
}

