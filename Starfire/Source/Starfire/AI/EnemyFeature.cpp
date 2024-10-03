// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFeature.h"
#include "Sf_NPCharacter.h"

void UEnemyFeature::Initialize(ASf_NPCharacter* Holder)
{
	this->OwningAIHolder = Holder;
}

FVector UEnemyFeature::CalculatePositionDeviation(FVector Position, float Accuracy)
{
	float X = CalculateDeviationValue(Position.X,Accuracy);
	float Y = CalculateDeviationValue(Position.Y,Accuracy);
	float Z = CalculateDeviationValue(Position.Z,Accuracy);

	return FVector(X,Y,Z);
}

FRotator UEnemyFeature::CalculateRotationDeviation(FRotator Rotator, float Accuracy)
{
	float Pitch = CalculateDeviationValue(Rotator.Pitch, Accuracy);
	float Yaw = CalculateDeviationValue(Rotator.Yaw,Accuracy);
	float Roll = CalculateDeviationValue(Rotator.Roll,Accuracy);

	return FRotator(Pitch,Yaw,Roll);
}

float UEnemyFeature::CalculateDeviationValue(float ValueToDeviateFrom,float Accuracy)
{
	Accuracy = Accuracy+0.000001f;
	return ValueToDeviateFrom+FMath::RandRange(-1/Accuracy,1/Accuracy);
}
