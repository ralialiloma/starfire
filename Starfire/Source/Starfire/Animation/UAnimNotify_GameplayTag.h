#pragma once
#include "UAnimNotify_GameplayTag.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_WeaponAnimationReceiver, Display, Display);

UCLASS(BlueprintType,Abstract,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class UAnimNotify_GameplayTag: public UAnimNotify
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable,Blueprintable)
	float PlayMontage(UAnimMontage* MontageToPlay) const;

	UFUNCTION(BlueprintCallable,Blueprintable)
	void StopMontage(UAnimMontage* MontageToStop, float BlendOutTime = 0) const;
};
