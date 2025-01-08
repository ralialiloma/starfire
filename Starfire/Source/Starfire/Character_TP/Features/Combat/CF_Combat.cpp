#include "CF_Combat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/WeaponConfig.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"



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
	bool ShowDebug = USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat,EDebugVisualType::Visual); 

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

	return true;
}
bool UCF_Combat::WouldHitPlayer(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit)
		return false;

	const AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
		return false;

	if (!HitActor->IsA(ASf_FP_Character::StaticClass()))
		return false;

	return true;
}

bool UCF_Combat::WouldHitNPC(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit)
		return false;

	const AActor* HitActor = HitResult.GetActor();
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

void UCF_Combat::OnEndPlay_Implementation(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireHandle);
	}
}

bool UCF_Combat::StartFire(const int MaxMissedBulletsBeforeStop, const bool bInClearFocusAfterFiring)
{
	bCLearFocusAfterFiring = bInClearFocusAfterFiring;
	if (bIsFiring)
		return false;
	if (SF_SHOULD_DEBUG(TP::CharacterFeatures::Combat,EDebugVisualType::Log))
		UE_LOG(EF_Combat, Log, TEXT("%s started firing"), *GetOwningActor()->GetName())
	if (SF_SHOULD_DEBUG(TP::CharacterFeatures::Combat,EDebugVisualType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2,
			FColor::Yellow,
			FString::Printf( TEXT("%s started firing"),
				*GetOwningActor()->GetName()));
	bIsFiring = true;
	GetOwningCharacter()->UnCrouch();
	GetOwningAIController()->SetFocus(USf_FunctionLibrary::GetSfPlayerpawn(this));
	DoFire(EInputSignalType::InputSignal_Started,MaxMissedBulletsBeforeStop);
	const float FireDelay = GetOwningSfEquipment()->GetWeaponConfig().FireDelay+0.01f;
	UE_LOG(EF_Combat, Log, TEXT("Starting timer with this rate %f"),FireDelay);
	GetWorld()->GetTimerManager().ClearTimer(FireHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FireHandle,
		[this,MaxMissedBulletsBeforeStop]()
		{
			if (this)	
				DoFire(EInputSignalType::InputSignal_Triggered,MaxMissedBulletsBeforeStop);
		},
		FireDelay,
		true,
		FireDelay);

	return true;
}

void UCF_Combat::StopFire()
{                                                        
	StopFire(FStopFireInfo(EStopFireReason::ManualInput));
}

void UCF_Combat::StopFire(FStopFireInfo StopFireInfo)
{
	GetWorld()->GetTimerManager().ClearTimer(FireHandle);
	if (!bIsFiring)
		return;

	LastFireStopInfo = StopFireInfo;
	
	FiredBullets = 0;
	MissedBullets = 0;

	if (bCLearFocusAfterFiring)
		GetOwningAIController()->ClearFocus(EAIFocusPriority::Gameplay);

	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat, EDebugVisualType::Log))
		UE_LOG(EF_Combat, Log, TEXT("Stopped Firing due to %s"), *StopFireInfo.ToString());
	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Combat, EDebugVisualType::Print) && GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			2,
			FColor::Green,
			FString::Printf(TEXT("Stopped Firing due to %s"), *StopFireInfo.ToString()));

	bIsFiring = false;
	OnFireStopped_CPP.Broadcast();
	OnFireStopped_BP.Broadcast();
}

void UCF_Combat::DoFire(const EInputSignalType InputSignalType, int MissedBulletsBeforeStop)
{
	USf_Equipment* Equipment = GetOwningSfEquipment();

	if(!IsValid(Equipment))
		return;
	
	const EFireType FireType = EFireType::FireType_Default;
	FHitResult HitResult;
	EFireBlock FireBlock;
	const bool bHasFired = Equipment->Fire(InputSignalType,FireType,HitResult,FireBlock);
	
	if (WouldHitNPC(HitResult))
	{
		StopFire(FStopFireInfo(EStopFireReason::HitOtherNPC));
		return;
	}

	if (!WouldHitPlayer(HitResult))
	{
		MissedBullets++;
		if(MissedBullets>=MissedBulletsBeforeStop)
		{
			StopFire(FStopFireInfo(EStopFireReason::CannotHitPlayer));
		}
		return;
	}
	
	if (bHasFired)
	{
		FiredBullets++;
		return;
	}

	//StopFire(FStopFireInfo(EStopFireReason::FireBlock,FireBlock));
};

void UCF_Combat::StartReload()
{
	

	if (bIsFiring)
		StopFire(FStopFireInfo(EStopFireReason::StartedReload));
	
	USf_Equipment* Equipment = GetOwningSfEquipment();
	if (!IsValid(Equipment))
		return;
	AWeaponBase* ActiveWeapon = Equipment->GetActiveWeapon();
	if (!IsValid(ActiveWeapon))
		return;
	ActiveWeapon->OnReloadFinish_BP.AddUniqueDynamic(this, &UCF_Combat::OnReloadFinish);
	ActiveWeapon->OnReloadStopped_BP.AddUniqueDynamic(this, &UCF_Combat::OnReloadStopped);

	float OutMontageTime  = 0;
	const bool bReloadSuccess =  Equipment->Reload(OutMontageTime);
	if (!bReloadSuccess)
	{
		StopReload();
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

FStopFireInfo UCF_Combat::GetLastStopFireInfo()
{
	return LastFireStopInfo;
}

void UCF_Combat::OnReloadStopped()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();
	if (IsValid(Equipment))
	{
		AWeaponBase* ActiveWeapon = Equipment->GetActiveWeapon();
		if (IsValid(ActiveWeapon))
		{
			if (OnReloadStoppedHandle.IsValid())
				ActiveWeapon->OnReloadStopped_CPP.Remove(OnReloadStoppedHandle);
			if (OnReloadFinishHandle.IsValid())
				ActiveWeapon->OnReloadFinish_CPP.Remove(OnReloadFinishHandle);
		}
	}
	OnReloadStopped_CPP.Broadcast();
	OnReloadStopped_BP.Broadcast();
}

void UCF_Combat::OnReloadFinish()
{
	USf_Equipment* Equipment = GetOwningSfEquipment();

	if (IsValid(Equipment))
	{
		AWeaponBase* ActiveWeapon = Equipment->GetActiveWeapon();
		if (IsValid(ActiveWeapon))
		{
			ActiveWeapon->OnReloadFinish_BP.RemoveDynamic(this, &UCF_Combat::OnReloadFinish);
			ActiveWeapon->OnReloadStopped_BP.RemoveDynamic(this, &UCF_Combat::OnReloadStopped);
		}
	}
	OnReloadFinish_CPP.Broadcast();
	OnReloadFinish_BP.Broadcast();
}
