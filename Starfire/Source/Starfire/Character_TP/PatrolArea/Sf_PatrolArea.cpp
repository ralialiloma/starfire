// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PatrolArea.h"

#include "Sf_PatrolAreaManager.h"
#include "Components/BrushComponent.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


ASf_PatrolArea::ASf_PatrolArea()
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

void ASf_PatrolArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASf_PatrolArea::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	USf_PatrolAreaManager* PatrolAreaManager = USf_PatrolAreaManager::Get(GetWorld());
	if (IsValid(PatrolAreaManager))
		PatrolAreaManager->RegisterPatrolArea(this);
}

bool ASf_PatrolArea::IsInBox(const FVector& LocationToTest) const
{

	const FTransform BoxTransform = Box->GetComponentTransform();
	const FVector LocalLocation = BoxTransform.InverseTransformPosition(LocationToTest);
	const FVector BoxExtent = Box->GetUnscaledBoxExtent(); 
	bool bIsInBox = FMath::Abs(LocalLocation.X) <= BoxExtent.X &&
					 FMath::Abs(LocalLocation.Y) <= BoxExtent.Y &&
					 FMath::Abs(LocalLocation.Z) <= BoxExtent.Z;

	return bIsInBox;
}

void ASf_PatrolArea::BeginPlay()
{
	Super::BeginPlay();
}

void ASf_PatrolArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

