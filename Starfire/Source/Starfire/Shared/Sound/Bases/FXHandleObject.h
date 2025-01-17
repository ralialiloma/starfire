// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXHandle.h"
#include "UObject/Object.h"
#include "FXHandleObject.generated.h"

class UFXDataAssetBase;

USTRUCT()
struct FFXComponentArrayWrapper
{
	GENERATED_BODY()

public:
	
	TArray<TWeakObjectPtr<USceneComponent>> Components {};
};

UCLASS()
class STARFIRE_API UFXHandleObject : public UObject
{
	GENERATED_BODY()

public:

	void Cancel();
	bool CanBeCanceled() const;

protected:

	friend class UFXSubsystem;

	UFUNCTION()
	void OnCompleteData(UWorld* World, FFXHandle Handle);
	UFUNCTION()
	void OnTimerCancel();
	UFUNCTION()
	float GetMaxCancelTime() const;

	UPROPERTY()
	FFXHandle FXHandle {};
	UPROPERTY()
	TMap<TObjectPtr<UFXDataAssetBase>, FFXComponentArrayWrapper> FXAssetComponentPairings {};
	UPROPERTY()
	bool Cancelled = false;

	UPROPERTY()
	UWorld* RelatedWorld = nullptr;
	UPROPERTY()
	FTimerHandle TimerHandle {};
};
