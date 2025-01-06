#include "WeaponAnimData_Weapon.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Sf_GameplayTagUtil.h"

FWeaponAnimData_Weapon::FWeaponAnimData_Weapon()
{
	TMap<FGameplayTag, UAnimMontage*>();
	UpdateEntries();
}

void FWeaponAnimData_Weapon::UpdateEntries()
{
	FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap<UAnimMontage*>(
		AnimationMontages,
		Sf_GameplayTags::Animation::Weapon::Montage::Name);
}


UWeaponAnimationAsset_Weapon::UWeaponAnimationAsset_Weapon()
{
}

#if WITH_EDITOR
void UWeaponAnimationAsset_Weapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}

void UWeaponAnimationAsset_Weapon::Refresh()
{
	WeaponAnimData.UpdateEntries();
}
#endif





