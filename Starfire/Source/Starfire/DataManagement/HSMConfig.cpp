#include "HSMConfig.h"

FHSMConfig::FHSMConfig()
{
	UFunctionLibrary::ValidateAndUpdateEnumMap<EEquipmentFlags,TSubclassOf<UBaseState>>(EquipmentFlagMap);
}

void UHSMConfiguration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UFunctionLibrary::ValidateAndUpdateEnumMap<EEquipmentFlags,TSubclassOf<UBaseState>>(EquipmentFlagStruct.EquipmentFlagMap);
}
