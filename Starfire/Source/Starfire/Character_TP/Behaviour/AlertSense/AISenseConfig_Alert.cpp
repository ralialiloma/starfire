// Fill out your copyright notice in the Description page of Project Settings.


#include "AISenseConfig_Alert.h"
#include "AISense_Alert.h"
#include "GameFramework/Actor.h"


UAISenseConfig_Alert::UAISenseConfig_Alert(const FObjectInitializer& ObjectInitializer): Radius(1000)
{
}

TSubclassOf<UAISense> UAISenseConfig_Alert::GetSenseImplementation() const
{
	return UAISense_Alert::StaticClass();
}
