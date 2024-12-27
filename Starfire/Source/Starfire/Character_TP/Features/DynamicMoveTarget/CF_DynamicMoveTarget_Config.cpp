
#include "CF_DynamicMoveTarget_Config.h"
#include "CF_DynamicMoveTarget.h"


TSubclassOf<USf_CharacterFeature> UCF_DynamicMoveTarget_Config::GetFeatureType_Implementation() const
{
	return UCF_DynamicMoveTarget::StaticClass();
}

