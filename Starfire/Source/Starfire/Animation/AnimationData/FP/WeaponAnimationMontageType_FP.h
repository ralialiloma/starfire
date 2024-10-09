#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationMontageType_FP: uint8
{
	None = 0 UMETA(Hidden),
	PrimaryFire = 1,
	SecondaryFire = 2,
	Melee = 3,
	Reload = 4,
	Equip = 5,
};

