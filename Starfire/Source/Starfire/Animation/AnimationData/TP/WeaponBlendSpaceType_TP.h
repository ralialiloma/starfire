#pragma once

UENUM(Blueprintable)
enum class EWeaponBlendSpaceType_TP: uint8
{
	None = 0 UMETA(Hidden),
	Walk = 1,
	Crouch = 2,
	WeaponHip = 3,
	WeaponAim = 4,
	WeaponRun = 5,
};

