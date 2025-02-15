	#include "WF_GripSocket_Config.h"
#include "WF_GripSocket.h"

	TSubclassOf<UWeaponFeature> UWF_GripSocket_Config::GetFeatureType_Implementation() const
	{
		return UWF_GripSocket::StaticClass(); 
	}
