// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PointLight.h"
#include "TutorialLight.generated.h"

class UTutorialConditions;

UCLASS(Blueprintable)
class STARFIRE_API ATutorialLight : public APointLight
{
	GENERATED_BODY()

protected:

	UPROPERTY(Instanced, BlueprintReadOnly, EditAnywhere)
	TArray<UTutorialConditions*> Conditions;
	
};
