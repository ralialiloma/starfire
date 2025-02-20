#pragma once
#include "LocomotionDirection.generated.h"
			

UENUM(BlueprintType)
enum class ELocomotionDirection: uint8
{
	None UMETA(Hidden),
	F UMETA(DisplayName = "Forward"),
	B UMETA(DisplayName = "Backward"),
	L UMETA(DisplayName = "Left"),
	R UMETA(DisplayName = "Right"),
};