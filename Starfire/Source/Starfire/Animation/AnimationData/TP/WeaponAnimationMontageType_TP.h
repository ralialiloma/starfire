#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationMontageType_TP: uint8
{
	None = 0 UMETA(Hidden),
	Fire = 1,
	Melee = 3,
	Reload = 4,
	Aim = 5,
	Equip = 6
};

