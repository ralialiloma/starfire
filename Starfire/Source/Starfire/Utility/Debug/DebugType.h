#pragma once
#include "DebugType.generated.h"

UENUM(BlueprintType,meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDebugType: uint8
{
	None    = 0,
	Print =  1 << 0,
	Log =  1 << 1,
	Visual =  1 << 2,
	Sound =  1 << 3
};
