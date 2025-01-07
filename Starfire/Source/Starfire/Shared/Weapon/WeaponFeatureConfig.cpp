#include "WeaponFeatureConfig.h"

#include "WeaponBase.h"
#include "WeaponFeature.h"


TSubclassOf<UWeaponFeature> UWeaponFeatureConfig::GetFeatureType_Implementation() const
{
	return nullptr;
}

UWeaponFeature* UWeaponFeatureConfig::MakeWeaponComponent(AWeaponBase* WeaponBase)
{
	if (!IsValid(WeaponBase))
		return nullptr;

	const TSubclassOf<UWeaponFeature> ComponentType = GetFeatureType();
	if (!IsValid(ComponentType))
		return nullptr;
	
	UWeaponFeature* NewComponent = NewObject<UWeaponFeature>(WeaponBase,ComponentType);
	if (!IsValid(NewComponent))
		return nullptr;

	NewComponent->Init(WeaponBase,this);
	return NewComponent;
}
