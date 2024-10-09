#pragma once
#include "WeaponMontageEventPackage.h"
#include "WeaponAnimMontageController.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_WeaponAnimationReceiver, Display, Display);

UCLASS(BlueprintType,Abstract,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class USf_WeaponAnimMontageController: public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable,Blueprintable)
	virtual float RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const;

protected:
	UFUNCTION(BlueprintCallable,Blueprintable)
	float PlayMontage(UAnimMontage* MontageToPlay) const;

	UFUNCTION(BlueprintCallable,Blueprintable)
	void StopMontage(UAnimMontage* MontageToStop, float BlendOutTime = 0) const;
};
