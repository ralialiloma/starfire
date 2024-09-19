#pragma once

UENUM(Blueprintable)
enum EWeaponAnimationMontageType: uint8
{
	AnimationMontage_None = 0 UMETA(Hidden),
	AnimationMontage_PrimaryFire = 1,
	AnimationMontage_SecondaryFire = 2,
	AnimationMontage_Melee = 3,
	AnimationMontage_Reload = 4,
};

