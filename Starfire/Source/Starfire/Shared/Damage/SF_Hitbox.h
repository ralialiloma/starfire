// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "SF_Hitbox.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STARFIRE_API USf_Hitbox : public UCapsuleComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category =  "Hitbox")
	float DamageMultiplier;

public:
	USf_Hitbox();
	
};
