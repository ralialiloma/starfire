#include "WeaponConfig.h"

#include "Kismet/KismetMathLibrary.h"

float FWeaponConfig::GetShotAngle(bool bIsAiming)
{
	float Accuracy = GetAccuracy(bIsAiming);
	float Randfloat = UKismetMathLibrary::RandomFloatInRange(0,1);
	float RandomAccuracy = UKismetMathLibrary:: MultiplyMultiply_FloatFloat(Randfloat,Accuracy);
	float Spread = GetSpread(bIsAiming);
	float ShotAngle = Spread*RandomAccuracy;
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
