#include "MapDataAsset.h"

void UHSMConfiguration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UFunctionLibrary::ValidateAndUpdateEnumMap<EEquipmentFlags,TSubclassOf<UBaseState>>(EquipmentFlagMap);
}
