// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sf_TetherPointGen.h"
#include "Components/ActorComponent.h"
#include "Sf_TetherPointSubsystem.generated.h"


UCLASS(BlueprintType, Blueprintable)
class STARFIRE_API USf_TetherPointSubsystem : public UGameInstanceSubsystem 
{
	GENERATED_BODY()


public:	
	UFUNCTION(BlueprintCallable)
	void RegisterTetherPointGen(ASf_TetherPointGen* ActorToRegister);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASf_TetherPointGen* GetTetherPointGen() const;

private:
	UPROPERTY()
	ASf_TetherPointGen* TetherPointGen = nullptr;
};
