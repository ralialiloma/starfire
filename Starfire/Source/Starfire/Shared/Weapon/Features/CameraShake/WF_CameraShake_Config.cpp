#include "WF_CameraShake_Config.h"
#include "WF_CameraShake.h"


TSubclassOf<UWeaponFeature> UWF_CameraShake_Config::GetFeatureType_Implementation() const
{
	return UWF_CameraShake::StaticClass();
}
