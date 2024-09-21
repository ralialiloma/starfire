// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "SkeletalDebugRendering.h"
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

void AWeaponBase::FireTraces(FTransform FireTransform, AActor* FiringActor)
{
	for (int i= 0; i< Config.BulletsPerShot;i++)
	{
		//Trace Points
		FVector Start,End;
		GetTracePoints(FireTransform,Start,End);

		//Debug
		EDrawDebugTrace::Type DebugType =
			UDebugSubsystem::GetWeaponDebug(Visual) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FColor TraceColor = UBlueprintFunctionLibrary::BoolToColor(bIsAiming);

		//Ignore
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>{FiringActor};

		//HitResult
		FHitResult HitResult;

		//Line Trace
		UKismetSystemLibrary::LineTraceSingle(
			this,
			Start,
			End,
			Config.TraceTypeQuery,
			false,
			ActorsToIgnore,
			DebugType,
			HitResult,
			true,
			TraceColor,
			FColor::Yellow,
			3.f);

		if (!HitResult.bBlockingHit)
			continue;

		//todo callHitVent

		UDamageReceiver* DamageReceiver = HitResult.GetActor()->GetComponentByClass<UDamageReceiver>();
		if (DamageReceiver==nullptr)
			continue;

		DamageReceiver->Apply
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

