#pragma once
#include "MovementStyle.generated.h"

UENUM(BlueprintType)
enum class EMovementStyle : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Alarmed UMETA(DisplayName = "Alarmed"),
};