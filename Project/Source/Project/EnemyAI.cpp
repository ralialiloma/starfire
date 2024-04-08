// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

FRotator AEnemyAI::CalucalteRotationDeviation(FRotator Rotator, float Accuracy)
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

