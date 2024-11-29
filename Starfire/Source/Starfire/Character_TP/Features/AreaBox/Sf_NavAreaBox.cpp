// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_NavAreaBox.h"
#include "Components/BrushComponent.h"


ASf_NavAreaBox::ASf_NavAreaBox()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a BrushComponent for the custom shape
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Box->SetBoxExtent(FVector(500,500,500));
	Box->ShapeColor = FColor::Purple;
	Box->SetLineThickness(10);
	RootComponent = Box;
	Box->SetHiddenInGame(true);
	Box->SetCanEverAffectNavigation(true);
	Box->bDynamicObstacle = true;
}

void ASf_NavAreaBox::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASf_NavAreaBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASf_NavAreaBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

