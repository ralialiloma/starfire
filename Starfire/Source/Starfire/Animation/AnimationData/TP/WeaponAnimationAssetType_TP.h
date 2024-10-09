#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationAssetType_TP: uint8
{
	None = 0 UMETA(Hidden),
	Idle= 1 ,
	JumpStart= 2 ,
	JumpEnd= 3,
	JumpLoop= 4,
};

