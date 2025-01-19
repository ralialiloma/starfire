// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegionVolume.h"
#include "Engine/TriggerBox.h"
#include "TutorialArea.generated.h"

class ATutorialManager;

UCLASS()
class STARFIRE_API ATutorialArea : public ARegionVolume
{
	GENERATED_BODY()

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(Transient)
	mutable ATutorialManager* TutorialManager = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess, Categories = "Gameplay.ActionLogger"))
	FGameplayTagContainer TagsToLogOnEnter {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess, Categories = "Gameplay.ActionLogger"))
	FGameplayTagContainer TagsToLogOnExit {};
};
