#include "HSMConfig.h"

#include "Starfire/Character/Movement/SF_CharacterMovementComponent.h"

FHSMConfig::FHSMConfig()
{
}

void UHSMConfiguration::Refresh()
{
	UFunctionLibrary::ValidateAndUpdateEnumMap<EEquipmentFlags,TSubclassOf<UBaseState>>(ConfigStruct.EquipmentFlagMap);
	UFunctionLibrary::ValidateAndUpdateEnumMap<EMovementMode,TSubclassOf<UBaseState>>(ConfigStruct.MovementModeFlag, TArray<TEnumAsByte<EMovementMode>>{EMovementMode::MOVE_Custom});
	UFunctionLibrary::ValidateAndUpdateEnumMap<ECustomMovementMode,TSubclassOf<UBaseState>>(ConfigStruct.CustomMovementModeFlag);
}

void UHSMConfiguration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Refresh();
}
