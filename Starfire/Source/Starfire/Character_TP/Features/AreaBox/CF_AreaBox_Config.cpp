
#include "CF_AreaBox_Config.h"
#include "CF_AreaBox.h"


TSubclassOf<USf_CharacterFeature> UCF_AreaBox_Config::GetFeatureType() const
{
	return UCF_AreaBox::StaticClass();
}

