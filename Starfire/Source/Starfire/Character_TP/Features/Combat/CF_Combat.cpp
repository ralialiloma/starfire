#include "CF_Combat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointGen.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/WeaponConfig.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"



bool UCF_Combat::OtherNPCWouldBeHit()
{
	if (!GetOwningCharacter()->Implements<UWeaponOwner>())
	{
		UE_LOG(
			EF_Combat, Error,
			TEXT("Character must implement %s to work with %s"),
			*UWeaponOwner::StaticClass()->GetName(),
			*StaticClass()->GetName())
		return false;
	}
	
	FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(GetOwningCharacter());
	FVector Start = FireTransform.GetLocation();
	FVector ForwardVector = FireTransform.GetRotation().GetForwardVector();
	const float WeaponRange= GetOwningSfEquipment()->GetWeaponConfig().Range;
	FVector End = ForwardVector*(WeaponRange+0.01f);
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{};
	bool ShowDebug = UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat,EDebugType::Visual); 

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

bool UCF_Combat::OtherNPCWouldBeHit(FHitResult HitResult)
{
	if (!HitResult.bBlockingHit)
		return false;

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
		return false;

	if (!HitActor->IsA(ASf_TP_Character::StaticClass()))
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

bool UCF_Combat::StartFire(bool bScoped, bool bInClearFocusAfterFiring)
{
	bCLearFocusAfterFiring = bInClearFocusAfterFiring;
	if (bIsFiring)
		return false;
	if (SHOULD_DEBUG(TP::CharacterFeatures::Combat,EDebugType::Log))
		UE_LOG(EF_Combat, Log, TEXT("%s started firing"), *GetOwningActor()->GetName())
	if (SHOULD_DEBUG(TP::CharacterFeatures::Combat,EDebugType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2,
			FColor::Yellow,
			FString::Printf( TEXT("%s started firing"),
				*GetOwningActor()->GetName()));
	bIsFiring = true;
	GetOwningCharacter()->UnCrouch();
	GetOwningAIController()->SetFocus(USf_FunctionLibrary::GetSfPlayerpawn(this));
	DoFire(EInputSignalType::InputSignal_Started,bScoped);
	const float FireDelay = GetOwningSfEquipment()->GetWeaponConfig().FireDelay+0.01f;
	UE_LOG(EF_Combat, Log, TEXT("Starting timer with this rate %f"),FireDelay);

	TWeakObjectPtr<UCF_Combat> WeakSelf = this;
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

	return true;
}

void UCF_Combat::StopFire()
{                                                        
	StopFire(FStopFireInfo(EStopFireReason::ManualInput));
}

void UCF_Combat::StopFire(FStopFireInfo StopFireInfo)
{
	if (!bIsFiring)
		return;
	
	FiredBullets = 0;

	if (bCLearFocusAfterFiring)
		GetOwningAIController()->ClearFocus(EAIFocusPriority::Gameplay);

	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat, EDebugType::Log))
		UE_LOG(EF_Combat, Log, TEXT("Stopped Firing due to %s"), *StopFireInfo.ToString());
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat, EDebugType::Print) && GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			2,
			FColor::Green,
			FString::Printf(TEXT("Stopped Firing due to %s"), *StopFireInfo.ToString()));

	bIsFiring = false;
	OnFireStopped_CPP.Broadcast();
	OnFireStopped_BP.Broadcast();
}

void UCF_Combat::DoFire(EInputSignalType InputSignalType, bool bScoped)
{
	USf_Equipment* Equipment = GetOwningSfEquipment();
	
	const EFireType FireType = bScoped?EFireType::FireType_Scoped:EFireType::FireType_Default;
	FHitResult HitResult;
	EFireBlock FireBlock;

	const bool bHasFired = Equipment->Fire(InputSignalType,FireType,HitResult,FireBlock);
	
	if (OtherNPCWouldBeHit(HitResult))
	{
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

void UCF_Combat::StartReload()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();

	if (bIsFiring)
		StopFire(FStopFireInfo(EStopFireReason::StartedReload));
	
	//On Reload Finish
	AWeaponBase* ActiveWeapon = Equipment->GetActiveWeapon();
	OnReloadFinishHandle = ActiveWeapon->OnReloadFinish_CPP.AddLambda([this,ActiveWeapon]()->void
	{
		if (IsValid(ActiveWeapon))
			ActiveWeapon->OnReloadFinish_CPP.Remove(OnReloadFinishHandle);
		OnReloadFinish_CPP.Broadcast();
		OnReloadFinish_BP.Broadcast();
	});

	//On Reload Stopped
	OnReloadStoppedHandle = ActiveWeapon->OnReloadStopped_CPP.AddLambda([this,ActiveWeapon]()->void
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

bool UCF_Combat::InstantReload()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();
	return Equipment->InstantReload();
}

void UCF_Combat::StopReload()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();
	Equipment->StopReloading();
	OnReloadStopped_CPP.Broadcast();
	OnReloadStopped_BP.Broadcast();
}

void UCF_Combat::DoIllegalReload()
{
	OnReloadFinish_BP.Broadcast();
	OnReloadFinish_CPP.Broadcast();
}

bool UCF_Combat::Melee()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();
	if (!Equipment->CanMelee())
		return false;

	const FVector PlayerLocation =  USf_FunctionLibrary::GetPlayerLocation(this);
	GetOwningAIController()->SetFocalPoint(PlayerLocation);
	return  Equipment->Melee();
}
