#pragma once

UENUM(Blueprintable)
enum EWeaponAnimationAssetType: uint8
{
	AnimationAsset_None = 0,
	AnimationAsset_Idle= 1,
	AnimationAsset_Run= 2,
	AnimationAsset_JumpStart= 3 ,
	AnimationAsset_JumpEnd= 4,
	AnimationAsset_JumpLoop= 5,
	AnimationAsset_Walk = 6,
};

