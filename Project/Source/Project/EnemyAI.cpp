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
	int X = CalculateDeviationValue(Accuracy, Position.X);
	int Y = CalculateDeviationValue(Accuracy, Position.X);
	int Z = CalculateDeviationValue(Accuracy, Position.X);

	return FVector(X,Y,Z);
}

float AEnemyAI::CalculateDeviationValue(float ValueToDeviateForm,float Accuracy)
{
	return FMath::RandRange(-1/Accuracy,1/Accuracy);
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

