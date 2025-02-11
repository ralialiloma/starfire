#pragma once
#include "RotationMode.generated.h"

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	OrientToMovement UMETA(DisplayName = "OrientToMovement"),
	Strafing UMETA(DisplayName = "Strafing"),
};