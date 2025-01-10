#include "WeaponFeature.h"

void UWeaponFeature::Init(AWeaponBase* WeaponBase, UWeaponFeatureConfig* Config)
{
	OwningWeapon = WeaponBase;
	GenericConfig = Config;
	OnInit();
}


void UWeaponFeature::OnHitObject_Implementation(FHitResult HitResult)
{
}

UWeaponFeatureConfig* UWeaponFeature::GetGenericConfig()
{
	return  GenericConfig;
}

AWeaponBase* UWeaponFeature::GetOwningWeapon()
{
	return OwningWeapon;
}

void UWeaponFeature::OnInit_Implementation()
{
}

void UWeaponFeature::OnBeginPlay_Implementation()
{
}

void UWeaponFeature::OnEndPlay_Implementation(const EEndPlayReason::Type EndPlayReason)
{
}

void UWeaponFeature::OnTick_Implementation(float OnTick)
{
}

void UWeaponFeature::OnFire_Implementation()
{
}

void UWeaponFeature::OnReload_Implementation()
{
}

