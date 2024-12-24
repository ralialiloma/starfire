// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoCollectResource.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AAutoCollectResource::AAutoCollectResource()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(Mesh);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	Mesh->SetRelativeScale3D(FVector::One() * 0.5f);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AAutoCollectResource::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SphereComponent->SetSphereRadius(CollectRange * (1 / Mesh->GetRelativeScale3D().X));
	Mesh->SetSimulatePhysics(bPhysicsBased);
}

void AAutoCollectResource::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAutoCollectResource::OnOverlapEnter);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAutoCollectResource::OnOverlapExit);
}

void AAutoCollectResource::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!IsPlayerContained())
		return;
	
	FVector VectorToPlayer = ContainedPawn->GetActorLocation() - GetActorLocation();
	if (!bPhysicsBased)
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), ContainedPawn->GetActorLocation(), DeltaSeconds, InterpSpeed));
	}
	else
	{
		float Multiplier = pow(VectorToPlayer.Length() / DistanceThreshold, DistancePower);
		float DistanceMultiplier = 1;
		if (Multiplier != 0)
			DistanceMultiplier = 1 / Multiplier;
		Mesh->AddForce(CollectForce * VectorToPlayer.GetSafeNormal() * 10 * DistanceMultiplier + FVector::UpVector * 10 * DistanceMultiplier * 
		CollectVerticalForce, NAME_None, true);
	}

	
	bool bHasOverShot = VectorToPlayer.Dot(PreviousVectorToPlayer) < -0.9f;
	if (VectorToPlayer.Length() < PickupRange || bHasOverShot)
		OnCollect(GetActorLocation(), ContainedPawn);

	PreviousVectorToPlayer = VectorToPlayer;
}

bool AAutoCollectResource::IsPlayerContained() const
{
	return ContainedPawn != nullptr;
}

float AAutoCollectResource::EvaluateCurveAt(float Value) const
{
	if (CollectCurve)
	{
		return CollectCurve->GetFloatValue(Value);
	}

	return 0;
}

void AAutoCollectResource::OnOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsPlayerContained())
		return;
	
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		ContainedPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	}
}

void AAutoCollectResource::OnOverlapExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!IsPlayerContained())
		return;
	
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		ContainedPawn = nullptr;
	}
}
