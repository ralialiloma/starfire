#pragma once
#include "WeaponMontageEventPackage.h"
#include "Sf_AnimHelper.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_AnimHelper, Display, Display);

DECLARE_MULTICAST_DELEGATE_OneParam(FAnimNotify_GameplayTag_Event_CPP,FGameplayTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimNotify_GameplayTag_Event_BP,FGameplayTag,GameplayTag);

UCLASS(BlueprintType,Abstract,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class USf_AnimHelper: public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,Blueprintable)
	virtual float RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData);

	virtual void OnAnimNotifyEvent(FGameplayTag Tag);

	FAnimNotify_GameplayTag_Event_CPP AnimNotifyEvent_CPP;
	UPROPERTY(BlueprintAssignable)
	FAnimNotify_GameplayTag_Event_BP AnimNotifyEvent_BP;

protected:
	UFUNCTION(BlueprintCallable,Blueprintable)
	float PlayMontage(UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintCallable,Blueprintable)
	void StopMontage(UAnimMontage* MontageToStop, float BlendOutTime = 0);
};
