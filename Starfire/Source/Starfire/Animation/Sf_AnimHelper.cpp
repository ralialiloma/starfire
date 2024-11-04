#include "Sf_AnimHelper.h"
#include "WeaponMontageEventPackage.h"
#include "Starfire/Shared/Weapon/Interfaces/WeaponOwner.h"


float USf_AnimHelper::RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const
{
	return 0;
}

void USf_AnimHelper::OnAnimNotifyEvent(FGameplayTag Tag)
{
	AnimNotifyEvent_CPP.Broadcast(Tag);
	AnimNotifyEvent_BP.Broadcast(Tag);
}

float USf_AnimHelper::PlayMontage(UAnimMontage* MontageToPlay) const
{
	const AActor* WeaponOwner = GetOwner();
	if (!IsValid(WeaponOwner))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid Weapon Owner to play montage on"))
		return 0;
	}
	
	if (!IsValid(MontageToPlay))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid Montage to play"))
		return 0;
	}

	if (!(WeaponOwner->Implements<UWeaponOwner>()))
	{
		UE_LOG(SF_AnimHelper,
				Warning,
				TEXT("Actor Requires %s interface to play montage"),
			   *UWeaponOwner::StaticClass()->GetName())
		return 0;
	}

	UAnimInstance* AnimInstance =  IWeaponOwner::Execute_GetCharacterAnimInstance(WeaponOwner);

	if (!IsValid(AnimInstance))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid AnimInstance"))
		return 0;
	}

	UE_LOG(
		SF_AnimHelper,
		Log,
		TEXT("Playing Montage %s on %s "),
		*MontageToPlay->GetName(),
		*WeaponOwner->GetName());
	
	return AnimInstance->Montage_Play(
		MontageToPlay,
		1
		,EMontagePlayReturnType::MontageLength,
		0,
		true);
}

void USf_AnimHelper::StopMontage(UAnimMontage* MontageToStop, float BlendOutTime) const
{

	AActor* WeaponOwner = GetOwner();
	if (!IsValid(WeaponOwner))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid Weapon Holder to stop montage on"))
		return;
	}
	
	if (!IsValid(MontageToStop))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid Montage to stop"))
		return;
	}

	if (!WeaponOwner->Implements<UWeaponOwner>())
	{
		UE_LOG(SF_AnimHelper,
				Warning,
				TEXT("Actor Requires %s interface to stop montage on "),
			   *UWeaponOwner::StaticClass()->GetName())
		return;
	}

	UAnimInstance* AnimInstance =  IWeaponOwner::Execute_GetCharacterAnimInstance(WeaponOwner);

	if (!IsValid(AnimInstance))
	{
		UE_LOG(SF_AnimHelper, Warning, TEXT("Invalid AnimInstance To Stop Montage"))
		return;
	}

	
	if (!AnimInstance->Montage_IsActive(MontageToStop))
		return;

	AnimInstance->Montage_Stop(BlendOutTime,MontageToStop);
	
	UE_LOG(
		SF_AnimHelper,
		Log,
		TEXT("Stoping Montage %s on %s "),
		*MontageToStop->GetName(),
		*WeaponOwner->GetName());
}

