#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationAssetType_LB_TP: uint8
{
	None = 0 UMETA(Hidden),
	Idle= 1,
	IdleCrouch = 2,
	Land = 3,
	Fall= 4,
};

