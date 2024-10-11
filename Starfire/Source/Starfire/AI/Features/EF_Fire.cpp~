#include "EF_Fire.h"

#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


bool UEF_Fire::OtherNPCWouldBeHit()
{

	const float WeaponRange= GetOwningSfEquipment()->GetWeaponConfig().Range;
	
	return false;
}

void UEF_Fire::StartFire(bool bScoped)
{
	if (bIsFiring)
		return;

	bIsFiring = true;
	GetOwningCharacter()->UnCrouch();
	GetOwningAIController()->SetFocalPoint(USf_FunctionLibrary::GetPlayerLocation(this));
	DoFire(EInputSignalType::InputSignal_Started,bScoped);
	const float FireDelay = GetOwningSfEquipment()->GetWeaponConfig().FireDelay;
	//Start FIre Timer
	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		[bScoped,this]()->void{this->DoFire(EInputSignalType::InputSignal_Triggered,bScoped);},
		FireDelay+0.01f,
		true
		);
}

void UEF_Fire::StopFire()
{                                                           
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void UEF_Fire::DoFire(EInputSignalType InputSignalType, bool bScoped)
{
	const EFireType FireType = bScoped?EFireType::FireType_Scoped:EFireType::FireType_Default;

	FHitResult HitResult;
	TEnumAsByte<EFireBlock> FireBlock;
	const bool bHasFired =  GetOwningSfEquipment()->Fire(InputSignalType,FireType,HitResult,FireBlock);

	if (bHasFired)
	{
		FiredBullets++;
		return;
	}

	switch (FireBlock)
	{
	case EFireBlock::None:
		break;
	case EFireBlock::Reload:
	case EFireBlock::EmptyClip:
	case EFireBlock::NotEnoughAmmo:
		// Update Has To Reload
		StopFire();
		break;
	case EFireBlock::Jammed:
	case EFireBlock::NoWeapon:
		StopFire();
		break;
	default:
		break;
	}
}
