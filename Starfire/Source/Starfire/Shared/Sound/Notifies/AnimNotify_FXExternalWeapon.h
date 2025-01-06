// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_FX.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FXExternalWeapon.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "FX External Weapon")
class STARFIRE_API UAnimNotify_FXExternalWeapon : public UAnimNotify_FX
{
	GENERATED_BODY()

public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
