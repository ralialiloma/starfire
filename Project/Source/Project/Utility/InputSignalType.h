#pragma once

UENUM(BlueprintType, meta=(Bitflags))
enum  EInputSignalType
{
	InputSignal_None = 0 UMETA(Hidden),
	InputSignal_Started = 1 UMETA(DisplayName="Started"),
	InputSignal_Triggered = 2 UMETA(DisplayName="Triggered"),
	InputSignal_Completed  = 3 UMETA(DisplayName="Completed") 
};
