// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KnockbackReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UKnockbackReceiver : public UInterface
{
	GENERATED_BODY()
};


class PROJECT_API IKnockbackReceiver: public IInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "KnockbackReceiver")
	void ReceiveKnockback(FVector KnockbackForce) const;
};