// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerComponent.generated.h"


UCLASS(Abstract,ClassGroup=(Player), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class STARFIRE_API UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
