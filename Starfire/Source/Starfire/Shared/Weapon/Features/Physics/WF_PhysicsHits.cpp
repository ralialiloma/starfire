#include "WF_PhysicsHits.h"

#include "WF_PhysicsHits_Config.h"
#include "Engine/StaticMeshActor.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_PhysicsHits::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_PhysicsHits_Config,Config);
}

void UWF_PhysicsHits::OnHitObject_Implementation(FHitResult HitResult) 
{
	Super::OnHitObject_Implementation(HitResult);

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
	if (StaticMeshComponent && StaticMeshComponent->Mobility == EComponentMobility::Movable)
	{
		StaticMeshComponent->SetSimulatePhysics(true);
		StaticMeshComponent->AddImpulse((HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal() * 1000.f * Config->ForceModifier);
	}
}