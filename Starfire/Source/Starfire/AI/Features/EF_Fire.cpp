#include "EF_Fire.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


bool UEF_Fire::OtherNPCWouldBeHit()
{
	ASf_NPCharacter* OwningCharacter = GetOwningCharacter();
	
	if (!OwningCharacter->Implements<UWeaponOwner>())
	{
		UE_LOG(
			EF_Fire, Error,
			TEXT("Character must implement %s to work with %s"),
			*UWeaponOwner::StaticClass()->GetName(),
			*StaticClass()->GetName())
		return false;
	}
	
	FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(OwningCharacter);
	FVector Start = FireTransform.GetLocation();
	FVector ForwardVector = FireTransform.GetRotation().GetForwardVector();
	const float WeaponRange= GetOwningSfEquipment()->GetWeaponConfig().Range;
	FVector End = ForwardVector*(WeaponRange+0.01f);
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{};
	bool ShowDebug = UDebugSubsystem::GetAIDebug(EDebugType::Visual);

	UKismetSystemLibrary::LineTraceSingleByProfile(
		this,
		Start,
		End,
		"BulletShots",
		false,
		GetIgnoreActors(),
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FColor::Red,
		FColor::Green,
		ShowDebug?1:0
	);

	return OtherNPCWouldBeHit(HitResult);

}

bool UEF_Fire::OtherNPCWouldBeHit(FHitResult HitResult)
{
	if (!HitResult.bBlockingHit)
		return false;

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
		return false;

	if (!HitActor->IsA(ASf_NPCharacter::StaticClass()))
		return false;

	return true;
}



void UEF_Fire::StartFire(bool bScoped)
{
	if (bIsFiring)
		return;

	bIsFiring = true;
	GetOwningCharacter()->UnCrouch();
	GetOwningAIController()->SetFocalPoint(USf_FunctionLibrary::GetPlayerLocation(this));
	bool bHitOtherNPC = false;
	DoFire(EInputSignalType::InputSignal_Started,bScoped, bHitOtherNPC);
	if (bHitOtherNPC)
		StopFire();
	const float FireDelay = GetOwningSfEquipment()->GetWeaponConfig().FireDelay;
	//Start Fire Timer
	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		[bScoped,this,&bHitOtherNPC]()->void{this->DoFire(EInputSignalType::InputSignal_Triggered,bScoped, bHitOtherNPC);},
		FireDelay+0.01f,
		true
		);
}

void UEF_Fire::StopFire()
{                                                           
	bIsFiring = false;
	FiredBullets = 0;
	if (!GetWorld()->GetTimerManager().TimerExists(FireTimer))
		return;

	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	OnFireStopped_CPP.Broadcast();
	OnFireStopped_BP.Broadcast();
}

void UEF_Fire::StartReload()
{
	USF_Equipment* Equipment = GetOwningSfEquipment();

	if (bIsFiring)
		StopFire();

	float OutMontageTime  = 0;
	bool bReloadSuccess =  Equipment->Reload(OutMontageTime);
	if (!bReloadSuccess)
	{
		StopReload();
		return;
	}

	//On Reload Finish
	AWeaponBase* ActiveWeapon = Equipment->GetActiveWeapon();
	FDelegateHandle OnReloadFinishHandle =  ActiveWeapon->OnReloadFinish_CPP.AddLambda([this,ActiveWeapon,OnReloadFinishHandle]()->void
	{
		if (IsValid(ActiveWeapon))
			ActiveWeapon->OnReloadFinish_CPP.Remove(OnReloadFinishHandle);
		OnReloadFinish_CPP.Broadcast();
		OnReloadFinish_BP.Broadcast();
	});

	//On Reload Stopped
	FDelegateHandle OnReloadStoppedHandle =  ActiveWeapon->OnReloadStopped_CPP.AddLambda([this,ActiveWeapon,OnReloadStoppedHandle]()->void
	{
		if (IsValid(ActiveWeapon))
			ActiveWeapon->OnReloadStopped_CPP.Remove(OnReloadStoppedHandle);
		OnReloadStopped_CPP.Broadcast();
		OnReloadStopped_BP.Broadcast();
	});
}

void UEF_Fire::StopReload()
{
	USF_Equipment* Equipment = GetOwningSfEquipment();
	Equipment->StopReloading();
	OnReloadStopped_CPP.Broadcast();
	OnReloadStopped_BP.Broadcast();
}

void UEF_Fire::DoFire(EInputSignalType InputSignalType, bool bScoped, bool& bHitOtherNPC)
{
	bHitOtherNPC = false;
	USF_Equipment* Equipment = GetOwningSfEquipment();
	
	const EFireType FireType = bScoped?EFireType::FireType_Scoped:EFireType::FireType_Default;
	FHitResult HitResult;
	TEnumAsByte<EFireBlock> FireBlock;

	const bool bHasFired = Equipment->Fire(InputSignalType,FireType,HitResult,FireBlock);


	if (OtherNPCWouldBeHit(HitResult))
	{
		bHitOtherNPC = true;
		StopFire();
		return;
	}
	
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
};