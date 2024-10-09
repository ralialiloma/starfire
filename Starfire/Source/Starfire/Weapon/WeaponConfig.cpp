#include "WeaponConfig.h"

#include "Kismet/KismetMathLibrary.h"
#include "Starfire/Utility/DebugSettings.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponConfig, Display, Display);

FWeaponConfig::FWeaponConfig()
{
}

float FWeaponConfig::GetShotAngle(bool bIsAiming) const
{
	float CurrentAccuracy = GetAccuracy(bIsAiming);
	float Randfloat = UKismetMathLibrary::RandomFloat();
	float RandomAccuracy = UKismetMathLibrary:: MultiplyMultiply_FloatFloat(Randfloat,CurrentAccuracy);
	float CurrentSpread = GetSpread(bIsAiming);
	float ShotAngle = CurrentSpread*RandomAccuracy;
	return ShotAngle;
}

float FWeaponConfig::GetAimAccuracy() const
{
	return  bUseMultipliers? Accuracy*AimAccuracyMultiplier:AimAccuracy;
}

float FWeaponConfig::GetAccuracy(bool bIsAiming) const
{
	return (bIsAiming?GetAimAccuracy():Accuracy)*AccuracyMultiplier;
}

float FWeaponConfig::GetSpread(bool bIsAiming) const
{
	return bIsAiming?GetAimSpread():Spread;
}

float FWeaponConfig::GetAimSpread() const
{
	return (bUseMultipliers?Spread*SpreadMultiplier:AimSpread)*SpreadMultiplier;
}

FWeaponAnimData_FP FWeaponConfig::GetAnimData_FP() const
{
	if (!IsValid(WeaponAnimationAsset_FPS))
	{
		UE_LOG(WeaponConfig, Log, TEXT("Missing Animation Asset"))
		return FWeaponAnimData_FP();
	}
	return WeaponAnimationAsset_FPS->WeaponAnimData;
}

FWeaponAnimData_TP FWeaponConfig::GetAnimData_TP() const
{
	if (!IsValid(WeaponAnimationAsset_TP))
	{
		UE_LOG(WeaponConfig, Log, TEXT("Missing Animation Asset"))
		return FWeaponAnimData_TP();
	}
	return WeaponAnimationAsset_TP->WeaponAnimData;
}
