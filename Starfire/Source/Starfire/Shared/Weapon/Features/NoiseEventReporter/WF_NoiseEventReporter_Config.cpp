#include "WF_NoiseEventReporter_Config.h"
#include "WF_NoiseEventReporter.h"


TSubclassOf<UWeaponFeature> UWF_NoiseEventReporter_Config::GetFeatureType_Implementation() const
{
	return UWF_NoiseEventReporter::StaticClass();
}
