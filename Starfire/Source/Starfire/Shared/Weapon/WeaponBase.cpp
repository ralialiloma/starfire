// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Interfaces/KnockbackReceiver.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Animation/WeaponMontageEventPackage.h"
#include "Starfire/Animation/Sf_AnimHelper.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


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

void AWeaponBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWeaponBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
                            FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Hit Item");
	
	if (bDestroyOnCollision)
		Destroy();
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
	if (!CanFire(InputSignal,FireType,OutFireBlock))
	{
		if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Weapon::FireBlocks,EDebugType::Print))
		{
			const FString FireBlockString = USf_FunctionLibrary::GetEnumAsString<EFireBlock>(OutFireBlock);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, "Could not fire due to "+FireBlockString);
		}
		
		return false;
	}

	StopReloading();
	
	DoFire(OutHitResult);
	ApplyRecoil();
	
	FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(WeaponOwner);
	if (!FireTransform.Equals(FTransform::Identity))
	{
		//Make Fire Transform relative to the root component
		FireTransform = GetRootComponent()->GetComponentTransform().GetRelativeTransform(FireTransform);
	}
	USceneComponent* FXAttachPoint = GetMuzzleSceneComponent();
	if (!FXAttachPoint)
		FXAttachPoint = GetRootComponent();
	
	UFXSubsystem::Get(this)->PlayFXOn(GetWeaponConfig().FXFireTag, FXAttachPoint);
	
	return true;
}

USceneComponent* AWeaponBase::GetMuzzleSceneComponent_Implementation() const
{
	return nullptr;
}

void AWeaponBase::FireTraces(FHitResult& OutHitResult)
{
	int BulletsPerShot = WeaponConfig.BulletsPerShot;
	if (BulletsPerShot <= 0)
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid BulletsPerShot value: %d"), WeaponConfig.BulletsPerShot);
		return;
	}
	
	for (int i= 0; i<BulletsPerShot;i++)
	{
		TraceALongFireTransform(OutHitResult);
		ApplyDamage(OutHitResult);
		//todo callHitEvent
	}
}

void AWeaponBase::TraceALongFireTransform(FHitResult& OutHitResult)
{
	
	//Trace Points
	FVector Start,End;
	const FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(WeaponOwner); 
	GetTracePoints(FireTransform,Start,End);

	//Debug
	bool bShouldDebug = USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Weapon::Name,EDebugType::Visual) ;
	const EDrawDebugTrace::Type DebugType =
		bShouldDebug? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	
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
}

void AWeaponBase::ApplyDamage(const FHitResult& InHitResult) const
{
	if (!InHitResult.bBlockingHit)
		return;
		
	USf_DamageController* DamageReceiver = InHitResult.GetActor()->GetComponentByClass<USf_DamageController>();
	if (DamageReceiver==nullptr)
		return;;

	if (!InHitResult.Component.IsValid())
		return;;
	
	DamageReceiver->ApplyDamage(WeaponConfig.Damage,InHitResult.Location,InHitResult.Normal,InHitResult.Component.Get(),
		WeaponConfig.DamageType);
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
	if (!CanAim())
		return;
	
	ExecuteAnimation(EWeaponAnimationEventType::Aim,true);
	bIsAiming = true;
}

void AWeaponBase::StopAiming()
{
	if (!CanAim())
		return;
	ExecuteAnimation(EWeaponAnimationEventType::Aim,false);
	bIsAiming = false;
}


void AWeaponBase::DoMelee()
{
	const float AnimLength =  ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType::Melee);
	bIsMeleeing = true;
	bActiveMeleeCountdown = false;

	for (AActor* Actor: OverlappedMeleeActors)
		ApplyMeleeToActor(Actor);

	FTimerDelegate EndMeleeDel;
	EndMeleeDel.BindLambda([this]() -> void
	{
		bIsMeleeing = false;
		bActiveMeleeCountdown = true;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() -> void { bActiveMeleeCountdown = false; });
		float CurrentMeleeDelay = WeaponConfig.MeleeDelay;
		if (CurrentMeleeDelay>0)
			GetWorld()->GetTimerManager().SetTimer(MeleeCooldown, TimerDel,CurrentMeleeDelay,false);
		else
			TimerDel.Execute();
	});

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, EndMeleeDel,AnimLength,false);
}



float AWeaponBase::ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType WeaponAnimationEventType, const bool bIsStarting) const
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	if (!IsValid(WeaponOwner))
	{
		UE_LOG(SF_Weapon, Warning, TEXT("Invalid Weapon Holder to get %s from"),*UAnimationDataController::StaticClass()->GetName())
		return 0;
	}
	
	USf_AnimHelper* AnimMontageController =  IWeaponOwner::Execute_GetAnimDataHelper(WeaponOwner);

	if (!IsValid(AnimMontageController))
	{
		UE_LOG(
			SF_Weapon,
			Warning,
			TEXT("Invalid %s Weapon Holder to broadcast animation event on montage on"),
			*UAnimationDataController::StaticClass()->GetName())
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

bool AWeaponBase::CanAim()
{
	return true;
}


void AWeaponBase::DoFire(FHitResult& OutHitResult)
{
	//Start FirePlayer Delay
	const float CurrentFireDelay = WeaponConfig.FireDelay;
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

	//Play Shoot Montage
	ExecuteAnimation(EWeaponAnimationEventType::Fire);
}

bool AWeaponBase::Reload()
{
	float MontageTime = 0;
	return Reload(MontageTime);
}

bool AWeaponBase::CanReload()
{
	return !IsReloading();
}

bool AWeaponBase::Reload(float& OutMontageTime)
{
	if (!CanReload())
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

void AWeaponBase::HandleMeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsMeleeing)
	{
		OverlappedMeleeActors.Add(OtherActor);
		return;
	}

	ApplyMeleeToActor(OtherActor);

}

void AWeaponBase::HandleMeleeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedMeleeActors.Remove(OtherActor);
}

void AWeaponBase::ApplyMeleeToActor(AActor* ActorToApplyMeleeTo)
{
	FMeleeInfo MeleeInfo =  IWeaponOwner::Execute_GetMeleeInfo(WeaponOwner);
	UBoxComponent* BoxComponent = MeleeInfo.BoxComponent;

	FVector TraceStart = WeaponOwner->GetActorLocation();
	FVector TraceEnd = ActorToApplyMeleeTo->GetActorLocation();

	//Line Trace
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		WeaponConfig.TraceTypeQuery,
		false,
		TArray<AActor*>{this,WeaponOwner},
		USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Weapon::Name,EDebugType::Visual)?EDrawDebugTrace::Persistent:EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Blue,
		FLinearColor::Red);

	if (!HitResult.bBlockingHit)
		return;

	FVector Direction = HitResult.ImpactNormal;
	Direction.Normalize();

	FVector KnockbackDirection = (ActorToApplyMeleeTo->GetActorLocation() - WeaponOwner->GetActorLocation()).GetSafeNormal();
	KnockbackDirection.Normalize();
	//Apply Knockback
	if (ActorToApplyMeleeTo->Implements<UKnockbackReceiver>())
		IKnockbackReceiver:: Execute_ReceiveKnockback(ActorToApplyMeleeTo,KnockbackDirection*GetWeaponConfig().MeleeKnockbackForce);
	

	//Get Damage Receiver
	USf_DamageController* DamageReceiver =  ActorToApplyMeleeTo->GetComponentByClass<USf_DamageController>();
	if (!IsValid(DamageReceiver))
		return;

	//Apply Damage
	APPLY_DAMAGE(DamageReceiver,
	GetWeaponConfig().MeleeDamage,
	BoxComponent->GetComponentLocation(),
	HitResult.Normal,
	HitResult.Component.Get(),
	Melee);
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
	if (bIsMeleeing)
		return false;

	return true;
}

int AWeaponBase::GetAmmoCount() const
{
	return CurrentClip;
}

FTransform AWeaponBase::GetMuzzleTransform() const
{
	return  SkeletalMesh->GetBoneTransform("Ammo");
}
void AWeaponBase::OnPickup(USf_Equipment* NewHolder)
{
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SkeletalMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SkeletalMesh->SetRelativeTransform(FTransform());
	UE_LOG(SF_Weapon, Log, TEXT("Picked up %s"),*GetClass()->GetName())
}

void AWeaponBase::OnDrop()
{
	WeaponOwner = nullptr;
	SetCollisionAndPhysics(true);
	UE_LOG(SF_Weapon, Log, TEXT("Dropped %s"),*GetClass()->GetName())
}

void AWeaponBase::SetCollisionAndPhysics(const bool bActive)
{
	SkeletalMesh->SetCollisionEnabled(bActive?ECollisionEnabled::Type::QueryAndPhysics:ECollisionEnabled::Type::NoCollision);
	SkeletalMesh->SetSimulatePhysics(bActive);
	SkeletalMesh->SetNotifyRigidBodyCollision(bActive); 
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

bool AWeaponBase::IsActionAllowed(const FGameplayTag ActionType) const
{
	if (!ForbiddenActions.Contains(ActionType))
		return true;
	
	return false;
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

USkeletalMeshComponent* AWeaponBase::GetSkeletalMesh() const
{
	return SkeletalMesh;
}

APawn* AWeaponBase::GetWeaponOwner() const
{
	return WeaponOwner;
}

USf_Equipment* AWeaponBase::GetOwningSfEquipmentComp() const
{
	return OwningEquipmentComponent;
}

void AWeaponBase::SetWeaponActive(const bool Active, AWeaponBase* OtherWeapon)
{
	SetActorEnableCollision(Active);
	SetActorHiddenInGame(!Active);
}

void AWeaponBase::SetNewHolder(USf_Equipment* NewHolder)
{
	OwningEquipmentComponent = NewHolder;
	WeaponOwner = OwningEquipmentComponent->GetOwner<APawn>();
	
	//Wait For Overlap Events
	FMeleeInfo MeleeInfo =  IWeaponOwner::Execute_GetMeleeInfo(WeaponOwner);
	MeleeInfo.BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AWeaponBase::HandleMeleeBeginOverlap);
	MeleeInfo.BoxComponent->OnComponentEndOverlap.AddDynamic(this,&AWeaponBase::AWeaponBase::HandleMeleeEndOverlap);
}