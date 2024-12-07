// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PatrolArea.h"
#include "Sf_PatrolAreaManager.h"
#include "Kismet/KismetStringLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Features/Death/CF_Death.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


ASf_PatrolArea::ASf_PatrolArea()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	if (!IsValid(GetWorld()))
		return;
	if (!IsValid(GetWorld()->GetGameInstance()))
		return;
	
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

bool ASf_PatrolArea::IsOccupied() const
{
	return Character != nullptr;
}

void ASf_PatrolArea::OnRegisterMarker_Implementation(ASf_PatrolAreaMarker* NewMarker)
{
}

void ASf_PatrolArea::BeginPlay()
{
	Super::BeginPlay();
	ImportActors();
}

void ASf_PatrolArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASf_PatrolArea::OnUnregistermarker_Implementation(ASf_PatrolAreaMarker* OldMarker)
{
}

bool ASf_PatrolArea::TryRegisterMarker(ASf_PatrolAreaMarker* Marker)
{
	ValidateMarkers();
	if (!IsValid(Marker))
	{
		return false;
	}
	if (!IsInBox(Marker->GetActorLocation()))
	{
		return false;
	}
	if (Markers.Contains(Marker))
	{
		return false;
	}
	
	Markers.Add(Marker);
	ValidateMarkers();
	OnRegisterMarker(Marker);
	return true;
}


void ASf_PatrolArea::UnregisterMarker(ASf_PatrolAreaMarker* Marker)
{
	ValidateMarkers();
	TArray<ASf_PatrolAreaMarker*>UpdatedMarkers{};
	for (ASf_PatrolAreaMarker* CurrentItem: Markers)
	{
		if (!IsValid(CurrentItem))
			continue;

		if (CurrentItem != Marker)
			UpdatedMarkers.Add(CurrentItem);
	}
	UpdatedMarkers = Markers;
	ValidateMarkers();
}

TArray<ASf_PatrolAreaMarker*> ASf_PatrolArea::GetCurrentMarkers() const
{
	return Markers;
}

void ASf_PatrolArea::ValidateMarkers()
{
	TArray<ASf_PatrolAreaMarker*> RegisteredMarkers;
	TArray<ASf_PatrolAreaMarker*> ValidMarkers;
	for (ASf_PatrolAreaMarker* CurrentItem: Markers)
	{
		if (!IsValid(CurrentItem))
			continue;

		if (!IsInBox(CurrentItem->GetActorLocation()))
			continue;

		if (RegisteredMarkers.Contains(CurrentItem))
			continue;

		RegisteredMarkers.Add(CurrentItem);
		ValidMarkers.Add(CurrentItem);
	}
	Markers = ValidMarkers;
}

void ASf_PatrolArea::ImportActors()
{
	TArray<AActor*> ActorsInBox;
	GetActorsInsideBox(ActorsInBox);
	for (AActor* Actor: ActorsInBox)
	{
		ASf_PatrolAreaMarker* PatrolAreaMarker = Cast<ASf_PatrolAreaMarker>(Actor);
		if (IsValid(PatrolAreaMarker))
			TryRegisterMarker(PatrolAreaMarker);

		Character = Cast<ASf_TP_Character>(Actor);
		if(IsValid(Character) && IsValid(	Character->GetFeatureByClass<UCF_Death>()))
		{
			Character->GetFeatureByClass<UCF_Death>()->OnDeath_CPP.AddLambda([this]()
			{
				OnCharacterDeath();
			});
		}
		else
			OnCharacterDeath();
	}
}

void ASf_PatrolArea::OnCharacterDeath()
{
}

void ASf_PatrolArea::GetActorsInsideBox(TArray<AActor*>& OutActors) const
{
	if (!IsValid(Box))
	{
		UE_LOG(LogTemp, Warning, TEXT("Box component is not valid."));
		return;
	}
	
	FBoxSphereBounds BoxBounds = Box->Bounds;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	TArray<FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		BoxBounds.Origin,
		Box->GetComponentRotation().Quaternion(),
		ECC_WorldDynamic,
		FCollisionShape::MakeBox(Box->GetScaledBoxExtent()),
		QueryParams
	);
	
	for (const FOverlapResult& Result : OverlapResults)
	{
		if (AActor* OverlappingActor = Result.GetActor())
		{
			OutActors.Add(OverlappingActor);
		}
	}
}

