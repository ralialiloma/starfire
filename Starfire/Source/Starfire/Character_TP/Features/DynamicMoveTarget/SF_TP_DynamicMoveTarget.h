// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SF_TP_DynamicMoveTarget.generated.h"

UCLASS()
class STARFIRE_API ASf_TP_DynamicMoveTarget : public AActor
{
	GENERATED_BODY()

public:
	ASf_TP_DynamicMoveTarget();
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	UBoxComponent* DebugBox;
};
