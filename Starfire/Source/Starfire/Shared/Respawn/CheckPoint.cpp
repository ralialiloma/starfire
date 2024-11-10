// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

#include "Components/TextRenderComponent.h"
#include "Starfire/Shared/Core/Sf_GameState.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


ACheckPoint::ACheckPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	CheckTriggerPawnClass = ASf_FP_Character::StaticClass();
 
	RootComponent = BoxComponent = CreateDefaultSubobject<UBoxComponent>("Root");
	BoxComponent->SetHiddenInGame(true);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetBoxExtent(FVector::One() * 50);
	
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->SetUsingAbsoluteLocation(false);
	TextRenderComponent->SetUsingAbsoluteRotation(false);
	TextRenderComponent->SetUsingAbsoluteScale(false);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnBeginOverlap);
	TArray<AActor*> OverlappingActors;
	BoxComponent->GetOverlappingActors(OverlappingActors, CheckTriggerPawnClass);
	if (OverlappingActors.Num() >= 1)
	{
		OnBeginOverlap(BoxComponent, OverlappingActors[0], nullptr, -1, false, FHitResult());
	}
}

void ACheckPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(CheckTriggerPawnClass))
	{
		if (ASf_GameState* GameState = GetWorld()->GetGameState<ASf_GameState>())
		{
			GameState->SetCheckpoint(this);
			OnSetCheckPoint_Internal();
			if (!bRetriggerableCheckpoint)
				Destroy();
			else
				GameState->OnCheckPointChange.AddDynamic(this, &ACheckPoint::OnCheckPointChange);
		}
	}
}

void ACheckPoint::OnCheckPointChange(FTransform NewCheckpoint, FTransform OldCheckpoint)
{
	if (OldCheckpoint.Equals(GetTransform()) && !NewCheckpoint.Equals(GetTransform()))
	{
		if (ASf_GameState* GameState = GetWorld()->GetGameState<ASf_GameState>())
			GameState->OnCheckPointChange.RemoveDynamic(this, &ACheckPoint::OnCheckPointChange);

		OnUnsetCheckPoint_Internal();
	}
}

void ACheckPoint::OnUnsetCheckPoint_Internal_Implementation()
{
}

void ACheckPoint::OnSetCheckPoint_Internal_Implementation()
{
}
