#include "WeaponConfig.h"

#include "Kismet/KismetMathLibrary.h"
#include "Project/Utility/DebugSettings.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponConfig, Display, Display);

FWeaponConfig::FWeaponConfig()
{
}

float FWeaponConfig::GetShotAngle(bool bIsAiming)
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

FWeaponAnimData FWeaponConfig::GetAnimData() const
{
	if (!IsValid(WeaponAnimationAsset))
	{
		UE_LOG(WeaponConfig, Log, TEXT("Missing Animation Asset"))
		return FWeaponAnimData();
	}
	return WeaponAnimationAsset->WeaponAnimData;
}
