// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LookForCoverWall.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ULookForCoverWall : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	

public:
	ULookForCoverWall();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TSubclassOf<UEnvQueryContext> Ignore;
	
	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TEnumAsByte<EDrawDebugTrace::Type> DebugDrawTraceType;
	
	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TSubclassOf<UEnvQueryContext> HideFrom;
	
};
