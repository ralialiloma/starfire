// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"

UPlayerComponent::UPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

