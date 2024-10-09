// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "FireBlocks.h"
#include "FireType.h"
#include "KnockbackReceiver.h"
#include "WeaponOwner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Animation/WeaponMontageEventPackage.h"
#include "Starfire/Animation/WeaponAnimMontageController.h"
#include "Starfire/Animation/AnimationData/WeaponAnimDataHelper.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/DamageSystem/Sf_DamageReceiver.h"
#include "Starfire/Utility/DebugSettings.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
	// Allow the actor to tick every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create and set DefaultSceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;
	RootComponent->Mobility = EComponentMobility::Type::Movable;

	// Create and attach SkeletalMesh component
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(DefaultSceneRoot);

	// Set the relative transform of the SkeletalMesh so it can be modified in the blueprint
	SkeletalMesh->SetRelativeLocation(FVector::ZeroVector);
	SkeletalMesh->SetRelativeRotation(FRotator::ZeroRotator);
	SkeletalMesh->SetRelativeScale3D(FVector(1.0f));

	// Ensure the SkeletalMesh uses relative transforms so offsets can be set in the blueprint
	SkeletalMesh->SetUsingAbsoluteLocation(false);
	SkeletalMesh->SetUsingAbsoluteRotation(false);
	SkeletalMesh->SetUsingAbsoluteScale(false);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentClip = WeaponConfig.MaxClipSize; 
}

void AWeaponBase::PostInitProperties()
{
	Super::PostInitProperties();
	
	CurrentClip = WeaponConfig.MaxClipSize; 
}

void AWeaponBase::OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
{
	IPrimaryInteract::OnInteractStart_Implementation(InteractComponent, TriggeringPawn);

	if (IsValid(TriggeringPawn))
	{
		if (USF_Equipment* EquipmentComp = TriggeringPawn->GetComponentByClass<USF_Equipment>())
		{
			int Slot = 0;
			EquipmentComp->AddWeapon(this, true, Slot);
		}
	}
}

bool AWeaponBase::Fire(const EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock) 
{
	StopReloading();
	
	if (!CanFire(InputSignal,FireType,OutFireBlock))
		return false;
	
	DoFire(OutHitResult);

	return true;
}

void AWeaponBase::FireTraces(FHitResult& OutHitResult)
{
	int BulletsPerShot = WeaponConfig.BulletsPerShot;
	if (BulletsPerShot <= 0)
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid BulletsPerShot value: %d"), WeaponConfig.BulletsPerShot);
		return;
	}

	const FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(WeaponOwner); 
	for (int i= 0; i<BulletsPerShot;i++)
	{
		//Trace Points
		FVector Start,End;
		GetTracePoints(FireTransform,Start,End);

		//Debug
		EDrawDebugTrace::Type DebugType =
			UDebugSubsystem::GetWeaponDebug(Visual) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FColor TraceColor = USf_FunctionLibrary::BoolToColor(bIsAiming);

		//Ignore
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>{WeaponOwner};
		

		//Line Trace
		UKismetSystemLibrary::LineTraceSingle(
			this,
			Start,
			End,
			WeaponConfig.TraceTypeQuery,
			false,
			ActorsToIgnore,
			DebugType,
			OutHitResult,
			true,
			TraceColor,
			FColor::Yellow,
			3.f);

		if (!OutHitResult.bBlockingHit)
			continue;
		
		USf_DamageReceiver* DamageReceiver = OutHitResult.GetActor()->GetComponentByClass<USf_DamageReceiver>();
		if (DamageReceiver==nullptr)
			continue;

		if (!OutHitResult.Component.IsValid())
			continue;

		DamageReceiver->ApplyDamage(
			WeaponConfig.Damage,
			OutHitResult.Location,
			OutHitResult.Normal,
			OutHitResult.Component.Get());

		//todo callHitEvent
	}
}

/*float AWeaponBase::PlayMontage(EWeaponAnimationMontageType_FP MontageType)
{
	UAnimMontage* MontageToPlay = UWeaponAnimDataHelper::GetAnimationMontage_TP(WeaponConfig.GetAnimData_FP(),	MontageType);

	if (!IsValid(MontageToPlay))
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Missing Weapon Montage For %s"), *UEnum::GetValueAsString(MontageType));
		return 0;
	}
	
	return PlayMontage(MontageToPlay);
}*/

void AWeaponBase::AimDownSight()
{
	ExecuteAnimation(EWeaponAnimationEventType::Aim,true);
	bIsAiming = true;
}

void AWeaponBase::StopAiming()
{
	ExecuteAnimation(EWeaponAnimationEventType::Aim,false);
	bIsAiming = false;
}


void AWeaponBase::DoMelee()
{
	ExecuteAnimation(EWeaponAnimationEventType::Melee);
	//PlayMontage(EWeaponAnimationMontageType_FP::Melee);

	float CurrentMeleeDelay = WeaponConfig.MeleeDelay;
	if (CurrentMeleeDelay>0)
	{
		//Start Cooldown
		bActiveMeleeCountdown = true;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() -> void { bActiveMeleeCountdown = false; });
		GetWorld()->GetTimerManager().SetTimer(MeleeCooldown, TimerDel,CurrentMeleeDelay,false);
	}
	MeleeTraces();
}

void AWeaponBase::MeleeTraces()
{
	if (!IsValid(WeaponOwner))
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid Weapon Holder to stop get melee info from"))
		return;
	}

	//Get Melee Info
	FMeleeInfo MeleeInfo =  IWeaponOwner::Execute_GetMeleeInfo(WeaponOwner);
	
	//Draw Debug
	if(UDebugSubsystem::GetWeaponDebug(EDebugType::Visual))
	{
		DrawDebugBox(
			GetWorld(),
			MeleeInfo.Location,
			MeleeInfo.Extent,
			MeleeInfo.Rotation.Quaternion(),
			FColor::Red,
			false,
			5.0f,
			0,           
			1.0f       
		);
	}

	TArray<AActor*> FoundActors{};
	TArray<AActor*> IgnoredActors{this,WeaponOwner};
	USf_FunctionLibrary::BetterBoxOverlapActors(
		this,
		MeleeInfo.Location,
		MeleeInfo.Rotation,
		MeleeInfo.Extent,
		 TArray<TEnumAsByte<EObjectTypeQuery>>{},
		AActor::StaticClass(),
		IgnoredActors,
		FoundActors);

	for (AActor* FoundActor: FoundActors)
	{
		//FVector Start = MeleeInfo.Location;
		FVector Origin;
		FVector BoxExtent;
		FoundActor->GetActorBounds(false,Origin,BoxExtent,true);
		const FVector End = MeleeInfo.Location*MeleeInfo.Direction*MeleeInfo.Extent*BoxExtent;
		ApplyMelee(FoundActor,MeleeInfo.Location,End, MeleeInfo.Direction);
	}
	
}

void AWeaponBase::ApplyMelee(AActor* ActorToApplyOn, FVector Start, FVector End, FVector Direction)
{

	//Apply Knockback
	if (ActorToApplyOn->Implements<UKnockbackReceiver>())
		IKnockbackReceiver:: Execute_ReceiveKnockback(ActorToApplyOn,Direction);

	//Get Damage Receiver
	USf_DamageReceiver* DamageReceiver =  ActorToApplyOn->GetComponentByClass<USf_DamageReceiver>();
	if (!IsValid(DamageReceiver))
		return;

	//Line Trace
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		WeaponConfig.TraceTypeQuery,
		false,
		TArray<AActor*>{this,WeaponOwner},
		UDebugSubsystem::GetWeaponDebug(EDebugType::Visual)?EDrawDebugTrace::Persistent:EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Blue,
		FLinearColor::Red);

	//Apply Damage
	DamageReceiver->ApplyDamage(
		GetWeaponConfig().MeleeDamage,
		HitResult.Location,
		HitResult.Normal,
		HitResult.Component.Get());
}


float AWeaponBase::ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType WeaponAnimationEventType, const bool bIsStarting) const
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	if (!IsValid(WeaponOwner))
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid Weapon Holder to get %s from"),*USf_WeaponAnimMontageController::StaticClass()->GetName())
		return 0;
	}
	
	USf_WeaponAnimMontageController* AnimMontageController =  IWeaponOwner::Execute_GetAnimMontageController(WeaponOwner);

	if (!IsValid(AnimMontageController))
	{
		UE_LOG(
			SF_Weapon,
			Warning,
			TEXT("Invalid %s Weapon Holder to broadcast animation event on montage on"),
			*USf_WeaponAnimMontageController::StaticClass()->GetName())
		return 0;
	}

	FWeaponMontageEventPackage WeaponAnimationUpdateData(
		WeaponAnimationEventType,
		bIsStarting,
		GetWeaponConfig().GetAnimData_FP(),
		GetWeaponConfig().GetAnimData_TP());
	return AnimMontageController->RunAnimation(WeaponAnimationUpdateData);
	
}

void AWeaponBase::ExecuteAnimation(EWeaponAnimationEventType WeaponAnimationEventType, bool bIsStarting) const
{
	float Time = ExecuteAnimationAndReturnAnimLength(WeaponAnimationEventType,bIsStarting);
}


void AWeaponBase::GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd)
{
	//Calculate Shot Angle
	float ShotAngle = WeaponConfig.GetShotAngle(bIsAiming);
	OutStart = InFireTransform.GetLocation();

	//Calculate Start And End Points
	UE::Math::TQuat<double> Rotation = InFireTransform.GetRotation();
	FVector ForwardVector =  Rotation.GetForwardVector()*WeaponConfig.Range;
	FVector UpVector = Rotation.GetUpVector();
	FVector ShotAngleVector = UKismetMathLibrary::RotateAngleAxis(ForwardVector,ShotAngle,UpVector);
	float RandomAngle = UKismetMathLibrary::RandomFloatInRange(0,360);
	FVector RandomShotAngleVector =
		UKismetMathLibrary::RotateAngleAxis(ShotAngleVector,RandomAngle,ForwardVector);
	OutEnd = OutStart+RandomShotAngleVector;
}


bool AWeaponBase::CheckInputSignalType(EInputSignalType InputSignalType)
{
	uint8 InputSignalValue = static_cast<uint8>(InputSignalType);
	return (WeaponConfig.AllowedInputSignals & InputSignalValue) == InputSignalValue;
}

bool AWeaponBase::IsOnFireCooldown()
{
	return GetWorld()->GetTimerManager().IsTimerActive(FireCooldown);
}

bool AWeaponBase::IsOnMeleeCooldown()
{
	return GetWorld()->GetTimerManager().IsTimerActive(MeleeCooldown);
}

bool AWeaponBase::IsAiming()
{
	return bIsAiming;
}


void AWeaponBase::DoFire(FHitResult& OutHitResult)
{
	//Start Fire Delay
	float CurrentFireDelay = WeaponConfig.FireDelay;
	if (CurrentFireDelay>0)
	{
		//Start Cooldown
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() -> void { bActiveFireCooldown = false;});
		GetWorld()->GetTimerManager().SetTimer(FireCooldown, TimerDel,CurrentFireDelay,false);
	}

	//Reduce Clip
	CurrentClip -= WeaponConfig.bInfiniteAmmo?0:1;

	//Shoot Traces
	FireTraces(OutHitResult);

	//todo callshoot event

	//Play Shoot Montage
	ExecuteAnimation(EWeaponAnimationEventType::Fire);
	//PlayMontage(EWeaponAnimationMontageType_FP::PrimaryFire);
	
}

bool AWeaponBase::Reload()
{
	if (IsReloading())
		return false;
	
	//float MontageTime = PlayMontage(EWeaponAnimationMontageType_FP::Reload);
	const float MontageTime = ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType::Reload,true);

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this]() -> void {CurrentClip = WeaponConfig.MaxClipSize;});
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer,TimerDel,MontageTime,false);

	return true;
}

bool AWeaponBase::IsReloading() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(ReloadTimer);
}

void AWeaponBase::StopReloading()
{
	if (!IsReloading())
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	ReloadTimer.Invalidate();
	
	ExecuteAnimation(EWeaponAnimationEventType::Reload,false);
	//UAnimMontage* ReloadMontage =  UWeaponAnimDataHelper::GetAnimationMontage_FP(GetWeaponConfig().GetAnimData_FP(), EWeaponAnimationMontageType_FP::Reload);
	//StopMontage(ReloadMontage);
}

bool AWeaponBase::Melee()
{
	if (!CanMelee())
		return false;
	
	DoMelee();
	return true;
}

bool AWeaponBase::CanMelee()
{
	if (!IsValid(WeaponOwner))
		return false;
	if(IsAiming())
		return false;
	if (bActiveMeleeCountdown)
		return false;
	if (bActiveFireCooldown)
		return false;

	return true;
}

int AWeaponBase::GetAmmoCount() const
{
	return CurrentClip;
}

FTransform AWeaponBase::GetMuzzleTransform() const
{
	return SkeletalMesh->GetRelativeTransform();
}

void AWeaponBase::OnPickup(AActor* NewHolder)
{
	WeaponOwner = NewHolder;
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	UE_LOG(SF_Weapon, Log, TEXT("Picked up %s"),*GetClass()->GetName())
}

void AWeaponBase::OnDrop()
{
	WeaponOwner = nullptr;
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	UE_LOG(SF_Weapon, Log, TEXT("Dropped %s"),*GetClass()->GetName())
}

void AWeaponBase::OnEquip()
{
	const float MontageTime = ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType::Equip,true);
	//PlayMontage(EWeaponAnimationMontageType_FP::Equip);
	UE_LOG(SF_Weapon, Log, TEXT("Equipped %s"),*GetClass()->GetName())
}

void AWeaponBase::OnUnequip()
{
	UE_LOG(SF_Weapon, Log, TEXT("Unequipped %s"),*GetClass()->GetName())
}

bool AWeaponBase::CanFire(EInputSignalType InputSignal, EFireType FireType,TEnumAsByte<EFireBlock>& OutBlock)
{
	//WeaponHolder
	if (!IsValid(WeaponOwner))
	{
		OutBlock = EFireBlock::Error;
		UE_LOG(SF_Weapon, Error, TEXT("Invalid Weapon Holder"))
		return false;
	}
	
	//Input Signal
	if (!CheckInputSignalType(InputSignal))
	{
		OutBlock = EFireBlock::TriggerType;
		return false;
	}

	//Clip
	if (CurrentClip<=WeaponConfig.AmmoCost && !WeaponConfig.bInfiniteAmmo)
	{
		OutBlock = CurrentClip==0?EFireBlock::EmptyClip:EFireBlock::NotEnoughAmmo;
		return false;
	}

	//Cooldown
	if (IsOnFireCooldown())
	{
		OutBlock = EFireBlock::FireCooldown;
		return false;
	}

	if (IsOnMeleeCooldown())
	{
		OutBlock = EFireBlock::MeleeCooldown;
		return false;
	}

	OutBlock = EFireBlock::None;
	return true;
}

FWeaponConfig AWeaponBase::GetWeaponConfig() const
{
	return  WeaponConfig;
}

AActor* AWeaponBase::GetWeaponOwner() const
{
	return WeaponOwner;
}

void AWeaponBase::SetWeaponActive(bool Active)
{
	SetActorEnableCollision(Active);
	SetActorHiddenInGame(!Active);
}


