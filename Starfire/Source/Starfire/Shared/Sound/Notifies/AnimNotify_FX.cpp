﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FX.h"

#include "DebugFunctionLibrary.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Starfire/Shared/Sound/Bases/FXHandle.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

UAnimNotify_FX::UAnimNotify_FX()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Purple;
#endif
}

FString UAnimNotify_FX::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("FX [%s]"),*USf_FunctionLibrary::RemoveParentTagsFromTag(FXTag, HiddenFXTag)); 
}

void UAnimNotify_FX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{	
	UFXSubsystem* FXSubsystem = UFXSubsystem::Get();
	if (!FXSubsystem)
		return;

	switch (PlayType)
	{
		case EFXPlayType::FX_Attached:
			if (AttachBone != NAME_None)
			{
				if (MeshComp->GetBoneIndex(AttachBone) != INDEX_NONE)
				{
					FXSubsystem->PlayFXOn(this, FXTag, MeshComp, AttachBone);
					break;
				}
			}
		case EFXPlayType::FX_2D:
			FXSubsystem->PlayFX(MeshComp, FXTag);
			break;
		default:
			UDebugFunctionLibrary::DebugError(this, "Notify Play type not supported!");
			break;
	}
}
