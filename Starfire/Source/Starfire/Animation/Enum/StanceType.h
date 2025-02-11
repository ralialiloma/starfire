#pragma once
#include "StanceType.generated.h"

UENUM(BlueprintType)
enum class EStanceType : uint8
{
	Stand UMETA(DisplayName = "Stand"),
	Crouch UMETA(DisplayName = "Crouch"),
};