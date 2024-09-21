// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
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

void AWeaponBase::FireTraces(FTransform FireTransform, AActor* FiringActor, FHitResult& OutHitResult)
{
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
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>{FiringActor};
		

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

bool AWeaponBase::IsInCooldown()
{
	return GetWorld()->GetTimerManager().IsTimerActive(FireCooldown);
}

void AWeaponBase::ResetFireCooldown()
{
	bActiveFireCooldown = false;
}


void AWeaponBase::Fire(
	FTransform InFireTransform,
	AActor* InActorFiring,
	FHitResult& OutHitResult,
	UAnimMontage*& OutMontageToPlay,
	float&OutRecoil)
{
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
	
	FireTraces(InFireTransform,InActorFiring, OutHitResult);

	//todo callshoot event

	//Find PrimaryFire Montage
	OutMontageToPlay =
		UWeaponAnimDataFunctions::GetAnimationMontage(
			CurrentWeaponAnimData,
			EWeaponAnimationMontageType::AnimationMontage_PrimaryFire);

	OutRecoil = Config.Recoil;	
}

void AWeaponBase::Reload(UAnimMontage*& OutMontageToPlay)
{
	CurrentClip = Config.MaxClipSize;
	OutMontageToPlay =
		UWeaponAnimDataFunctions::GetAnimationMontage(
			CurrentWeaponAnimData,
			EWeaponAnimationMontageType::AnimationMontage_Reload);
	
}

void AWeaponBase::OnEquip(UAnimMontage*& OutMontageToPlay, AActor* NewOwner)
{
	/*OutMontageToPlay =
		UWeaponAnimDataFunctions::GetAnimationMontage(
			CurrentWeaponAnimData,
			EWeaponAnimationMontageType::AnimationMontage_Equip);*/
}

