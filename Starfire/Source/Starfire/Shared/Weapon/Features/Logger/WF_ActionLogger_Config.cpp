#include "WF_ActionLogger_Config.h"
#include "WF_ActionLogger.h"


TSubclassOf<UWeaponFeature> UWF_ActionLogger_Config::GetFeatureType_Implementation() const
{
	return UWF_ActionLogger::StaticClass();
}
