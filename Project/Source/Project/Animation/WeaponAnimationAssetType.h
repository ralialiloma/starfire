#pragma once

UENUM(Blueprintable)
enum EWeaponAnimationAssetType: uint8
{
	AnimationAsset_None = 0 UMETA(Hidden),
	AnimationAsset_Idle= 2 ,
	AnimationAsset_JumpStart= 3 ,
	AnimationAsset_JumpEnd= 4,
	AnimationAsset_JumpLoop= 5,
};

