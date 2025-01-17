// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/PoseSnapshot.h"
#include "PoseAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class STARFIRE_API UPoseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetPose(FPoseSnapshot InSnapshot)
	{
		Snapshot = InSnapshot;
	}

protected:
	
	UPROPERTY(BlueprintReadOnly)
	FPoseSnapshot Snapshot;
};
