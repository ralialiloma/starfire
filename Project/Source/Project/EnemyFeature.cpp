// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFeature.h"
#include "EnemyAI.h"

void UEnemyFeature::Initialize(AEnemyAI* Holder)
{
	this->OwningAIHolder = Holder;
}
