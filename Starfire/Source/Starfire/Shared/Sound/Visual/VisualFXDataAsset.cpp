// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualFXDataAsset.h"

#include "DebugFunctionLibrary.h"
#include "VisualFXProcessor.h"
#include "Kismet/GameplayStatics.h"

FVisualFXSettings* UVisualFXDataAsset::GetVisualFXSettings(FGameplayTag Tag)
{
	return FXMap.Find(Tag);
}

void UVisualFXDataAsset::ExecuteFX_Implementation(UObject* WorldContext, FFXParams Params)
{
	if (!WorldContext || !WorldContext->GetWorld())
    {
        UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("World Context Invalid!")));
        return;
    }

    if (FVisualFXSettings* FoundEffect = GetVisualFXSettings(Params.FXTag))
    {
        if (!FoundEffect->IsValid())
        {
            UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Found visual effect data is invalid for GameplayTag: %s"), *Params.FXTag.ToString()));
            return;
        }

        UParticleSystemComponent* ParticleComp = nullptr;
        switch (Params.PlayType)
        {
            case EFXPlayType::FX_2D:
                UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Visual FX cannot be played on a 2D FX Type!")));
                break;
            case EFXPlayType::FX_Location:
                ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(
                    WorldContext->GetWorld(),
                    FoundEffect->ParticleSystem,
                    Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                    (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                    Params.Transform.GetScale3D() * FoundEffect->AdditiveTransform.GetScale3D(),
                    true,
                    EPSCPoolMethod::AutoRelease);
                break;
            case EFXPlayType::FX_Attached:
                ParticleComp = UGameplayStatics::SpawnEmitterAttached(
                    FoundEffect->ParticleSystem,
                    Params.AttacheSceneComponent,
                    Params.Bone,
                    Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                    (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                    Params.Transform.GetScale3D() * FoundEffect->AdditiveTransform.GetScale3D(),
                    EAttachLocation::KeepRelativeOffset,
                    true);
                break;
        }

        if (!ParticleComp)
        {
            return;
        }

        for (auto Processor : FoundEffect->Processors)
        {
            if (!Processor)
                continue;

            Processor->ModifyFX(ParticleComp);
        }
    }
}