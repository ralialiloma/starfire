#include "WF_HitVFX.h"

#include "WF_HitVFX_Config.h"
#include "Kismet/KismetMathLibrary.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_HitVFX::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_HitVFX_Config,Config);
}

void UWF_HitVFX::OnHitObject_Implementation(FHitResult HitResult)
{
	Super::OnHitObject_Implementation(HitResult);
	UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
	PlayHitVFX(PhysMaterial,HitResult.ImpactPoint);
}


void UWF_HitVFX::PlayHitVFX(const UPhysicalMaterial* PhysicalMaterial, const FVector& HitLocation) const
{
	if (!IsValid(Config))
		return;

	if (FGameplayTag* FoundMessage = Config->MaterialEffectPairings.Find(PhysicalMaterial))
	{
		UFXSubsystem::Get()->PlayFXAt(this, *FoundMessage, FTransform(HitLocation));
		return;
	}

	UFXSubsystem::Get()->PlayFXAt(this, Config->DefaultVFXMessage, FTransform(HitLocation));
}
