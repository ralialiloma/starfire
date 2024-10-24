// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Interfaces/KnockbackReceiver.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Animation/WeaponMontageEventPackage.h"
#include "Starfire/Animation/Sf_AnimDataController.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugSubsystem.h"


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
		if (USf_Equipment* EquipmentComp = TriggeringPawn->GetComponentByClass<USf_Equipment>())
		{
			int Slot = 0;
			EquipmentComp->AddWeapon(this, true, Slot);
		}
	}
}

bool AWeaponBase::Fire(const EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult,EFireBlock& OutFireBlock) 
{
	StopReloading();
	
	if (!CanFire(InputSignal,FireType,OutFireBlock))
		return false;
	
	DoFire(OutHitResult);
	ApplyRecoil();

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
		const EDrawDebugTrace::Type DebugType =
			UDebugSubsystem::GetWeaponDebug(Visual) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FColor TraceColor = USf_FunctionLibrary::BoolToColor(bIsAiming);

		//Ignore
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>{WeaponOwner,this};

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
		
		USf_DamageController* DamageReceiver = OutHitResult.GetActor()->GetComponentByClass<USf_DamageController>();
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

void AWeaponBase::ApplyRecoil(float Modifier) const
{
	if (!GetWeaponOwner() || !GetWeaponOwner()->GetController())
		return;

	APlayerController* PlayerController = Cast<APlayerController>(GetWeaponOwner()->GetController());
	if (!PlayerController)
		return;

	if (GetWeaponConfig().RecoilAngle != 0)
	{
		FRotator ControlRotation = PlayerController->GetControlRotation();
		ControlRotation.Pitch += GetWeaponConfig().RecoilAngle * Modifier;
		PlayerController->SetControlRotation(ControlRotation);
	}

	if (GetWeaponConfig().RecoilShake.IsValid()) //Use Async Load when weapon is loaded in for better performance
		PlayerController->PlayerCameraManager->StartCameraShake(GetWeaponConfig().RecoilShake.LoadSynchronous());
}

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
	USf_DamageController* DamageReceiver =  ActorToApplyOn->GetComponentByClass<USf_DamageController>();
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
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid Weapon Holder to get %s from"),*USf_AnimDataController::StaticClass()->GetName())
		return 0;
	}
	
	USf_AnimDataController* AnimMontageController =  IWeaponOwner::Execute_GetAnimMontageController(WeaponOwner);

	if (!IsValid(AnimMontageController))
	{
		UE_LOG(
			SF_Weapon,
			Warning,
			TEXT("Invalid %s Weapon Holder to broadcast animation event on montage on"),
			*USf_AnimDataController::StaticClass()->GetName())
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


void AWeaponBase::GetTracePoints(const FTransform& InFireTransform, FVector& OutStart, FVector& OutEnd) const
{
	// Calculate Shot Angle
	float ShotAngle = WeaponConfig.GetShotAngle(bIsAiming);
	OutStart = InFireTransform.GetLocation();

	// Get rotation from the fire transform
	UE::Math::TQuat<double> Rotation = InFireTransform.GetRotation();
	FVector ForwardVector = Rotation.GetForwardVector();
	FVector UpVector = Rotation.GetUpVector();

	// Apply shot angle offset
	FVector ShotAngleVector = UKismetMathLibrary::RotateAngleAxis(ForwardVector, ShotAngle, UpVector);
    
	// Apply random angle to the shot angle vector
	float RandomAngle = UKismetMathLibrary::RandomFloatInRange(0, 360.0f);
	FVector RandomShotAngleVector = UKismetMathLibrary::RotateAngleAxis(ShotAngleVector, RandomAngle, ForwardVector);

	// Calculate end point by normalizing the direction and multiplying by the range
	OutEnd = OutStart + RandomShotAngleVector.GetSafeNormal() * WeaponConfig.Range;
}


bool AWeaponBase::CheckInputSignalType(EInputSignalType InputSignalType)
{
	uint8 InputSignalValue = static_cast<uint8>(InputSignalType);
	return (WeaponConfig.AllowedInputSignals & InputSignalValue) == InputSignalValue;
}

bool AWeaponBase::IsOnFireCooldown()
{
	return bActiveFireCooldown;
}

bool AWeaponBase::HasEnoughAmmoToFire() const
{
	return  WeaponConfig.bInfiniteAmmo||CurrentClip>=WeaponConfig.AmmoCost;
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
		bActiveFireCooldown = true;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() -> void { bActiveFireCooldown = false;});
		GetWorld()->GetTimerManager().SetTimer(FireCooldown, TimerDel,CurrentFireDelay,false,CurrentFireDelay);
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
	float MontageTime = 0;
	return Reload(MontageTime);
}

bool AWeaponBase::Reload(float& OutMontageTime)
{
	if (IsReloading())
		return false;
	
	//float MontageTime = PlayMontage(EWeaponAnimationMontageType_FP::Reload);
	const float MontageTime = ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType::Reload,true);

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this]() ->void
	{
		CurrentClip = WeaponConfig.MaxClipSize;
		OnReloadFinish_BP.Broadcast();
		OnReloadFinish_CPP.Broadcast();
	});

	const float EffectiveMontageTime = FMath::Max(MontageTime, 0.001f);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer,TimerDel,EffectiveMontageTime,false);
	return true;
}

bool AWeaponBase::InstantReload()
{
	CurrentClip = WeaponConfig.MaxClipSize;
	OnReloadFinish_BP.Broadcast();
	OnReloadFinish_CPP.Broadcast();
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

	OnReloadStopped_BP.Broadcast();
	OnReloadStopped_CPP.Broadcast();
	
	ExecuteAnimation(EWeaponAnimationEventType::Reload,false);
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

void AWeaponBase::OnPickup(APawn* NewHolder)
{
	WeaponOwner = NewHolder;
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	UE_LOG(SF_Weapon, Log, TEXT("Picked up %s"),*GetClass()->GetName())
}

void AWeaponBase::OnDrop()
{
	WeaponOwner = nullptr;
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	SkeletalMesh->SetSimulatePhysics(true);
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

bool AWeaponBase::CanFire(const EInputSignalType InputSignal, EFireType FireType,EFireBlock& OutBlock)
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
	if (!HasEnoughAmmoToFire())
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

APawn* AWeaponBase::GetWeaponOwner() const
{
	return WeaponOwner;
}

void AWeaponBase::SetWeaponActive(bool Active)
{
	SetActorEnableCollision(Active);
	SetActorHiddenInGame(!Active);
}


