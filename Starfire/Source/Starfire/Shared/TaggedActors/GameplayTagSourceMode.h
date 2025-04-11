#pragma once
#include "GameplayTagSourceMode.generated.h"

UENUM(BlueprintType)
enum class EGameplayTagSourceMode : uint8
{
	StaticOnly     UMETA(DisplayName = "Static Only"),
	DynamicOnly    UMETA(DisplayName = "Dynamic Only"),
	Both           UMETA(DisplayName = "Both")
};