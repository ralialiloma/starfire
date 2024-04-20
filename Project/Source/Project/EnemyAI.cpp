// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UEnemyFeature* AEnemyAI::GetFeatureByClass( const TSoftClassPtr<UEnemyFeature> T)
{
	for (auto Element : Features)
	{
		if (Element->GetClass() == T)
		{
			return Element;
		}
	}

	return nullptr;
}

bool AEnemyAI::TryAddFeature(UEnemyFeature* Feature)
{
	for (auto Element : Features)
	{
		if (Element->GetClass() == Feature->GetClass())
		{
			return false;
		}
	}

	Features.Add(Feature);
	Feature->Initialize(this);
	
	return true;
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AEnemyAI::CalculatePositionDeviation(FVector Position, float Accuracy)
{
	float X = CalculateDeviationValue(Position.X,Accuracy);
	float Y = CalculateDeviationValue(Position.Y,Accuracy);
	float Z = CalculateDeviationValue(Position.Z,Accuracy);

	return FVector(X,Y,Z);
}

FRotator AEnemyAI::CalculateRotationDeviation(FRotator Rotator, float Accuracy)
{
	float Pitch = CalculateDeviationValue(Rotator.Pitch, Accuracy);
	float Yaw = CalculateDeviationValue(Rotator.Yaw,Accuracy);
	float Roll = CalculateDeviationValue(Rotator.Roll,Accuracy);

	return FRotator(Pitch,Yaw,Roll);
}

float AEnemyAI::CalculateDeviationValue(float ValueToDeviateFrom,float Accuracy)
{
	Accuracy = Accuracy+0.000001f;
	return ValueToDeviateFrom+FMath::RandRange(-1/Accuracy,1/Accuracy);
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

