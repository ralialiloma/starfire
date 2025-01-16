// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualFXDataAsset.h"

#include "DebugFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "VisualFXProcessor.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/StarFireGameplayTags.h"

DEFINE_LOG_CATEGORY(LogVisualFX);

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
        
        DEBUG_SIMPLE(LogVisualFX, Log, FColor::White, FString::Printf(TEXT("Playing %s"), *Params.FXTag.ToString()), Sf_GameplayTags::Effects::FX::VisualFX::Name);

        UFXSystemComponent* ParticleComp = nullptr;
        switch (Params.PlayType)
        {
            case EFXPlayType::FX_2D:
                UDebugFunctionLibrary::DebugError(this, FString::Printf(TEXT("Visual FX cannot be played on a 2D FX Type!")));
                break;
            case EFXPlayType::FX_Location:
                if (UParticleSystem* ParticleSystem = Cast<UParticleSystem>(FoundEffect->ParticleSystem))
                {
                    ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(
                        WorldContext->GetWorld(),
                        ParticleSystem,
                        Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                        (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                        Params.Transform.GetScale3D() * FoundEffect->AdditiveTransform.GetScale3D(),
                        true,
                        EPSCPoolMethod::AutoRelease);
                }
                if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(FoundEffect->ParticleSystem))
                {
                    ParticleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                        WorldContext->GetWorld(),
                        NiagaraSystem,
                        Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                        (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                        Params.Transform.GetScale3D() * FoundEffect->AdditiveTransform.GetScale3D(),
                        true,
                        true);
                }
                break;
            case EFXPlayType::FX_Attached:
                if (UParticleSystem* ParticleSystem = Cast<UParticleSystem>(FoundEffect->ParticleSystem))
                {
                    ParticleComp = UGameplayStatics::SpawnEmitterAttached(
                        ParticleSystem,
                        Params.AttacheSceneComponent,
                        Params.Bone,
                        Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                        (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                        Params.Transform.GetScale3D() * FoundEffect->AdditiveTransform.GetScale3D(),
                        EAttachLocation::KeepRelativeOffset,
                        true);
                }
                if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(FoundEffect->ParticleSystem))
                {
                    ParticleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
                        NiagaraSystem,
                        Params.AttacheSceneComponent,
                        Params.Bone,
                        Params.Transform.GetLocation() + FoundEffect->AdditiveTransform.GetLocation(),
                        (Params.Transform.GetRotation() + FoundEffect->AdditiveTransform.GetRotation()).Rotator(),
                        EAttachLocation::Type::SnapToTarget,
                        EAttachLocation::KeepRelativeOffset,
                        true);
                }
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