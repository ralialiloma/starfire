// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "FireBlocks.h"
#include "FireType.h"
#include "WeaponOwner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Project/DamageReceiver.h"
#include "Project/Utility/DebugSettings.h"
#include "Project/Utility/DebugSubsystem.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot =  CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SkeletalMesh =  CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeaponBase::Fire(const EInputSignalType InputSignal,
						EFireType FireType,
						FHitResult& OutHitResult,
						TEnumAsByte<EFireBlock>& OutFireBlock) 
{
	if (!CanFire(InputSignal,FireType,OutFireBlock))
		return false;
	
	DoFire(OutHitResult);
	return true;
}

void AWeaponBase::FireTraces(FHitResult& OutHitResult)
{

	FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(WeaponHolder); 
	for (int i= 0; i< Config.BulletsPerShot;i++)
	{
		//Trace Points
		FVector Start,End;
		GetTracePoints(FireTransform,Start,End);

		//Debug
		EDrawDebugTrace::Type DebugType =
			UDebugSubsystem::GetWeaponDebug(Visual) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FColor TraceColor = UFunctionLibrary::BoolToColor(bIsAiming);

		//Ignore
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>{WeaponHolder};
		

		//Line Trace
		UKismetSystemLibrary::LineTraceSingle(
			this,
			Start,
			End,
			Config.TraceTypeQuery,
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
		
		UDamageReceiver* DamageReceiver = OutHitResult.GetActor()->GetComponentByClass<UDamageReceiver>();
		if (DamageReceiver==nullptr)
			continue;

		if (!OutHitResult.Component.IsValid())
			continue;

		DamageReceiver->ApplyDamage(
			Config.Damage,
			OutHitResult.Location,
			OutHitResult.Normal,
			OutHitResult.Component.Get());

		//todo callHitEvent
	}
}

float AWeaponBase::PlayMontage(EWeaponAnimationMontageType MontageType)
{
	UAnimMontage* MontageToPlay =
		UWeaponAnimDataFunctions::GetAnimationMontage(
			CurrentWeaponAnimData,
			MontageType);
	return  PlayMontage(MontageToPlay);
}

void AWeaponBase::DoMelee()
{
	
}


float AWeaponBase::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (WeaponHolder==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Weapon Owner to play montage on"))
		return 0;
	}
	
	if (MontageToPlay==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Montage to play"))
		return 0;
	}

	if (!WeaponHolder->Implements<UWeaponOwner>())
	{
		UE_LOG(LogTemp,
				Warning,
				TEXT("Actor Requires %s interface to play shoot montage"),
			   *UWeaponOwner::StaticClass()->GetName())
		return 0;
	}

	UAnimInstance* AnimInstance =  IWeaponOwner::Execute_GetCharacterAnimInstance(WeaponHolder);
	
	return AnimInstance->Montage_Play(
		MontageToPlay,
		1
		,EMontagePlayReturnType::MontageLength,
		0,
		true);
}


void AWeaponBase::GetTracePoints(FTransform InFireTransform, FVector& OutStart, FVector& OutEnd)
{
	//Calculate Shot Angle
	float ShotAngle = Config.GetShotAngle(bIsAiming);
	OutStart = InFireTransform.GetLocation();

	//Calculate Start And End Points
	UE::Math::TQuat<double> Rotation = InFireTransform.GetRotation();
	FVector ForwardVector =  Rotation.GetForwardVector()*Config.Range;
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
	return (Config.AllowedInputSignals & InputSignalValue) == InputSignalValue;
}

bool AWeaponBase::IsInCooldown()
{
	return GetWorld()->GetTimerManager().IsTimerActive(FireCooldown);
}

void AWeaponBase::ResetFireCooldown()
{
	bActiveFireCooldown = false;
}


void AWeaponBase::DoFire(FHitResult& OutHitResult)
{
	//Start Fire Delay
	float CurrentFireDelay = Config.FireDelay;
	if (CurrentFireDelay>0)
	{
		//Start Cooldown
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() -> void { ResetFireCooldown(); });
		GetWorld()->GetTimerManager().SetTimer(FireCooldown, TimerDel,0.01f,false);
	}

	//Reduce Clip
	CurrentClip -= Config.bInfiniteAmmo?0:1;

	//Shoot Traces
	FireTraces(OutHitResult);

	//todo callshoot event

	//Play Shoot Montage
	PlayMontage(EWeaponAnimationMontageType::AnimationMontage_PrimaryFire);
	
}

float AWeaponBase::Reload()
{
	CurrentClip = Config.MaxClipSize;
	return PlayMontage(EWeaponAnimationMontageType::AnimationMontage_Reload);
}

float AWeaponBase::Melee()
{
	DoMelee();
	return 0;
}

int AWeaponBase::GetAmmoCount()
{
	return CurrentClip;
}

void AWeaponBase::OnEquip(AActor* NewHolder)
{
	SetWeapon(true);
	WeaponHolder = NewHolder;

	PlayMontage(EWeaponAnimationMontageType::AnimationMontage_Equip);
}

bool AWeaponBase::CanFire(EInputSignalType InputSignal, EFireType FireType,TEnumAsByte<EFireBlock>& OutBlock)
{
	//Input Signal
	if (CheckInputSignalType(InputSignal))
	{
		OutBlock = EFireBlock::TriggerType;
		return false;
	}

	//Clip
	if (CurrentClip<=Config.AmmoCost && !Config.bInfiniteAmmo)
	{
		OutBlock = CurrentClip==0?EFireBlock::EmptyClip:EFireBlock::NotEnoughAmmo;
		return false;
	}

	//Cooldown
	if (IsInCooldown())
	{
		OutBlock = EFireBlock::FireCooldown;
		return false;
	}

	OutBlock = EFireBlock::None;
	return true;
}

void AWeaponBase::SetWeapon(bool Active)
{
	SetActorEnableCollision(Active);
	SetActorHiddenInGame(!Active);
}

