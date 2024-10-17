#include "EF_Combat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


bool UEF_Combat::OtherNPCWouldBeHit()
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

bool UEF_Combat::OtherNPCWouldBeHit(FHitResult HitResult)
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


FStopFireInfo::FStopFireInfo(): StopFireReason(EStopFireReason::None)
                                , FireBlock(EFireBlock::None)
{
}

FStopFireInfo::FStopFireInfo(EStopFireReason InStopFireReason, EFireBlock InFireBlock)
{
	FireBlock = InFireBlock;
	StopFireReason = InStopFireReason;
}

FString FStopFireInfo::ToString()
{
	FString StopFireReasonString = USf_FunctionLibrary::GetEnumAsString<EStopFireReason>(StopFireReason);
	FString FireBlockString = USf_FunctionLibrary::GetEnumAsString<EFireBlock>(FireBlock);

	if (StopFireReason == EStopFireReason::FireBlock)
		return  FString::Printf(TEXT("StopFireReason: %s, Blocked by: %s"), *StopFireReasonString, *FireBlockString);
	else
		return  FString::Printf(TEXT("StopFireReason: %s"), *StopFireReasonString);
}

void UEF_Combat::StartFire(bool bScoped)
{
	if (bIsFiring)
		return;

	UE_LOG(LogTemp, Log, TEXT("I started firing"))
	bIsFiring = true;
	GetOwningCharacter()->UnCrouch();
	GetOwningAIController()->SetFocalPoint(USf_FunctionLibrary::GetPlayerLocation(this));
	DoFire(EInputSignalType::InputSignal_Started,bScoped);
	const float FireDelay = GetOwningSfEquipment()->GetWeaponConfig().FireDelay+0.01f;
	UE_LOG(LogTemp, Log, TEXT("Starting timer with this rate %f"),FireDelay);

	TWeakObjectPtr<UEF_Combat> WeakSelf = this;
	FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[bScoped,WeakSelf,FireDelay]()->void
	{
		if (!WeakSelf.IsValid() || !WeakSelf->bIsFiring)
			return;
		FAsyncUtility::WaitForSeconds(FireDelay,WeakSelf,0.05f);
		while (WeakSelf.IsValid() && WeakSelf->bIsFiring)
		{
			FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf,bScoped]()->void
			{
				if (!WeakSelf.IsValid())
					return;
				WeakSelf->DoFire(EInputSignalType::InputSignal_Triggered,bScoped);
			});
			FAsyncUtility::WaitForSeconds(FireDelay,WeakSelf,0.05f);
		}
	});
}

void UEF_Combat::StopFire()
{                                                        
	StopFire(FStopFireInfo(EStopFireReason::ManualInput));
}

void UEF_Combat::StopFire(FStopFireInfo StopFireInfo)
{
	if (bIsFiring == false)
		return;

	/*if (StopFireInfo.StopFireReason == EStopFireReason::FireBlock)
	{
		switch (StopFireInfo.FireBlock)
		{
		case EFireBlock::None:
			break;
		case EFireBlock::Reload:
		case EFireBlock::EmptyClip:
		case EFireBlock::NotEnoughAmmo:
			SetBlackboardBoolValue(EBoolBlackboardKey::HasToReload,true);
			break;
		case EFireBlock::Jammed:
			break;
		case EFireBlock::FireCooldown:
			break;
		case EFireBlock::TriggerType:
			break;
		case EFireBlock::Error:
			break;
		case EFireBlock::NoWeapon:
			break;
		case EFireBlock::MeleeCooldown:
			break;
		default: ;
		}
	}*/
	
	bIsFiring = false;
	FiredBullets = 0;
	UE_LOG(LogTemp, Log, TEXT("Stopped Firing due to %s"),*StopFireInfo.ToString())
	OnFireStopped_CPP.Broadcast();
	OnFireStopped_BP.Broadcast();
}

void UEF_Combat::DoFire(EInputSignalType InputSignalType, bool bScoped)
{
	USF_Equipment* Equipment = GetOwningSfEquipment();
	
	const EFireType FireType = bScoped?EFireType::FireType_Scoped:EFireType::FireType_Default;
	FHitResult HitResult;
	EFireBlock FireBlock;

	const bool bHasFired = Equipment->Fire(InputSignalType,FireType,HitResult,FireBlock);
	
	if (OtherNPCWouldBeHit(HitResult))
	{
		//UE_LOG(LogTemp, Log, TEXT("I hit another NPC"))
		StopFire(FStopFireInfo(EStopFireReason::HitOtherNPC));
		return;
	}
	
	if (bHasFired)
	{
		FiredBullets++;
		return;
	}

	StopFire(FStopFireInfo(EStopFireReason::FireBlock,FireBlock));
};

void UEF_Combat::StartReload()
{
	USF_Equipment* Equipment = GetOwningSfEquipment();

	if (bIsFiring)
		StopFire(FStopFireInfo(EStopFireReason::StartedReload));
	
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

	float OutMontageTime  = 0;
	const bool bReloadSuccess =  Equipment->Reload(OutMontageTime);
	if (!bReloadSuccess)
	{
		StopReload();
		return;
	}

}

void UEF_Combat::StopReload()
{
	USF_Equipment* Equipment = GetOwningSfEquipment();
	Equipment->StopReloading();
	OnReloadStopped_CPP.Broadcast();
	OnReloadStopped_BP.Broadcast();
}

bool UEF_Combat::Melee()
{
	USF_Equipment* Equipment = GetOwningSfEquipment();
	if (!Equipment->CanMelee())
		return false;

	const FVector PlayerLocation =  USf_FunctionLibrary::GetPlayerLocation(this);
	GetOwningAIController()->SetFocalPoint(PlayerLocation);
	return  Equipment->Melee();
}