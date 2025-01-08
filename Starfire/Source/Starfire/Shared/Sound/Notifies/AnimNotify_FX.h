// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/Sound/FXDataAssetBase.h"
#include "AnimNotify_FX.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "FX Notify")
class STARFIRE_API UAnimNotify_FX : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_FX();
	
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Effects.Messages"))
	FGameplayTag FXTag = FGameplayTag();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay, meta = (Categories = "Effects.Messages"))
	FGameplayTag HiddenFXTag = Sf_GameplayTags::Effects::Messages::Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InvalidEnumValues = "FX_Location"))
	EFXPlayType PlayType = EFXPlayType::FX_Attached;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachBone = NAME_None;
};
