// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BetterObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class PROJECT_API UBetterObject : public UObject
{
	GENERATED_BODY()
	
	//UObject interface implementation
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwningActor() const;

	UFUNCTION(BlueprintCallable)
	void DestroyObject();
	
protected:

	//Custom Construct and Destroy Event
	UFUNCTION(BlueprintImplementableEvent, Category = "Core")
	void OnConstruct();
	UFUNCTION(BlueprintImplementableEvent, Category = "Core")
	void OnDestroyed();

	virtual void PostInitProperties() override;
};
