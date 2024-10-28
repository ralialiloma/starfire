#include "HSMConfig.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

FHSMConfig::FHSMConfig()
{
}

void UHSMConfiguration::Refresh()
{
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EEquipmentFlags,TSubclassOf<UBaseState>>(ConfigStruct.EquipmentFlagMap);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EMovementMode,TSubclassOf<UBaseState>>(ConfigStruct.MovementModeFlag, TArray<TEnumAsByte<EMovementMode>>{EMovementMode::MOVE_Custom});
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<ECustomMovementMode,TSubclassOf<UBaseState>>(ConfigStruct.CustomMovementModeFlag);
}

#if WITH_EDITOR
void UHSMConfiguration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Refresh();
}
#endif
