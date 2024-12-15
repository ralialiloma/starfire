// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_TP_DynamicMoveTarget.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


ASf_TP_DynamicMoveTarget::ASf_TP_DynamicMoveTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	DebugBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DebugBox"));
	DebugBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	DebugBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugBox->SetVisibility(true);
	DebugBox->SetHiddenInGame(false);
	DebugBox->SetCollisionObjectType(ECC_WorldDynamic);
	DebugBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = DebugBox;
	DebugBox->ShapeColor = FColor::Green;
	DebugBox->SetLineThickness(5);
}


void ASf_TP_DynamicMoveTarget::BeginPlay()
{
	Super::BeginPlay();
	DebugBox->SetVisibility(SF_SHOULD_DEBUG(TP::DynamicMoveTarget,EDebugType::Visual));
	
}

