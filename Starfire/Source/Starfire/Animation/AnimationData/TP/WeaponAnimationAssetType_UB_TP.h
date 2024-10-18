#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationAssetType_UB_TP: uint8
{
	None = 0 UMETA(Hidden),
	Idle= 1,
	IdleCrouch = 2,
	Aim = 3,
	AimCrouch = 4
};

