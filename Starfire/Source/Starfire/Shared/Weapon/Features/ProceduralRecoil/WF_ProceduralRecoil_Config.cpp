	#include "WF_ProceduralRecoil_Config.h"

#include "WF_ProceduralRecoil.h"

	TSubclassOf<UWeaponFeature> UWF_ProceduralRecoil_Config::GetFeatureType_Implementation() const
	{
		return UWF_ProceduralRecoil::StaticClass(); 
	}
