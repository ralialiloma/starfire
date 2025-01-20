#include "WF_PhysicsHits_Config.h"
#include "WF_PhysicsHits.h"

TSubclassOf<UWeaponFeature> UWF_PhysicsHits_Config::GetFeatureType_Implementation() const
{
	return UWF_PhysicsHits::StaticClass(); 
}
