	#include "WF_HitVFX_Config.h"
	#include "WF_HitVFX.h"

	TSubclassOf<UWeaponFeature> UWF_HitVFX_Config::GetFeatureType_Implementation() const
	{
		return UWF_HitVFX::StaticClass(); 
	}
