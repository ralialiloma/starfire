// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnLocation.h"

#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


ARespawnLocation::ARespawnLocation()
{
	PrimaryActorTick.bCanEverTick = true;
 
	RootComponent =  CreateDefaultSubobject<USceneComponent>("Root");
	
	TextRenderComponent =  CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->SetUsingAbsoluteLocation(false);
	TextRenderComponent->SetUsingAbsoluteRotation(false);
	TextRenderComponent->SetUsingAbsoluteScale(false);
}

void ARespawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARespawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ARespawnLocation* ARespawnLocation::GetClosestSpawnLocation(UObject* WorldContextObject)
{
	FVector PlayerLocation =  USf_FunctionLibrary::GetPlayerLocation(WorldContextObject);
	
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject,ARespawnLocation::StaticClass(),FoundActors);
	AActor* ClosestActor = nullptr;
	float ClosestDistance = MAX_FLT;

	for (AActor* FoundActor : FoundActors)
	{
		float CurrentDistance = FVector::Dist(PlayerLocation, FoundActor->GetActorLocation());

		if (CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestActor = Cast<ARespawnLocation>(FoundActor);
		}
	}

	return Cast<ARespawnLocation>(ClosestActor);
}

FTransform ARespawnLocation::GetClosestRespawnTransform(UObject* WorldContextObject)
{
	const ARespawnLocation* SpawnLocation =  GetClosestSpawnLocation(WorldContextObject);
	if (IsValid(SpawnLocation))
		return SpawnLocation->GetTransform();
	return FTransform::Identity;
}

