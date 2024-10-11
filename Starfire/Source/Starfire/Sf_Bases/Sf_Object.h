// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sf_Object.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class STARFIRE_API USf_Object : public UObject
{
	GENERATED_BODY()
public:
	virtual UWorld* GetWorld() const override;
	virtual void PostInitProperties() override;
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwningActor() const;

	UFUNCTION(BlueprintCallable)
	void DestroyObject();
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Core")
	void OnConstruct();
	UFUNCTION(BlueprintImplementableEvent, Category = "Core")
	void OnDestroyed();
#pragma endregion
	


};
