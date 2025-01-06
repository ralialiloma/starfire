#pragma once
#include "DebugVisualType.generated.h"

UENUM(BlueprintType,meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDebugVisualType: uint8
{
	None    = 0,
	Print =  1 << 0,
	Log =  1 << 1,
	Visual =  1 << 2,
	Sound =  1 << 3
};
