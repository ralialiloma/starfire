// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AnimationModifier.h"
#include "AnimPose.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimData/IAnimationDataController.h"
#include "EngineLogs.h"
#include "Sf_CopyBonesModifier.generated.h"

USTRUCT(BlueprintType)
struct FSf_BoneReferencePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Default")
	FBoneReference SourceBone;

	UPROPERTY(EditAnywhere, Category = "Default")
	FBoneReference TargetBone;
};


/** Animation Modifier to copy the transform of 'SourceBone(s)' to 'TargetBone(s)' */
UCLASS()
class USf_CopyBonesModifier : public UAnimationModifier
{
	GENERATED_BODY()

public:

	/** Source and Target bone pairs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<FSf_BoneReferencePair> BonePairs;

	/** Space to convert SourceBone transforms into before copying them to TargetBone */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EAnimPoseSpaces BonePoseSpace = EAnimPoseSpaces::World;

	USf_CopyBonesModifier();

	virtual void OnApply_Implementation(UAnimSequence* Animation) override;
	virtual void OnRevert_Implementation(UAnimSequence* Animation) override;

};
