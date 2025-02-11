#pragma once
#include "MovementState.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Moving UMETA(DisplayName = "Moving"),
	Idle UMETA(DisplayName = "Idle"),
};