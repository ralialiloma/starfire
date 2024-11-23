// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Sf_AiSettings.generated.h"

UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "Starfire AI Settings"))
class STARFIRE_API USf_AISettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	static const USf_AISettings* Get();

public:
	UPROPERTY(BlueprintReadOnly,Config, EditAnywhere)
	TSoftObjectPtr<UEnvQuery> CloseToPlayerLocationQuery;
};