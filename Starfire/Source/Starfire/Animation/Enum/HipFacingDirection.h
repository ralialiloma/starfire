#pragma once
#include "HipFacingDirection.generated.h"
			

UENUM(BlueprintType)
enum class EHipFacingDirection: uint8
{
	None UMETA(Hidden),
	F UMETA(DisplayName = "Forward"),
	B UMETA(DisplayName = "Backward"),
};