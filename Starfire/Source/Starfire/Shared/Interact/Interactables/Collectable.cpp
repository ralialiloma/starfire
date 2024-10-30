#include "Collectable.h"

#include "GameFramework/Character.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY(SFCollectable);

ACollectable::ACollectable()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void ACollectable::OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
{
	if (bHasBeenCollected)
		return;
	
	IPrimaryInteract::OnInteractStart_Implementation(InteractComponent, TriggeringPawn);

	if (USf_Equipment* Equipment = TriggeringPawn->GetComponentByClass<USf_Equipment>())
	{
		if (ItemTag == Sf_GameplayTags::Gameplay::ItemType::Environment)
		{
			//TODO: Increase Resource
		}
		else if (ItemTag == Sf_GameplayTags::Gameplay::ItemType::Enemy)
		{
			//TODO: Increase Resource
		}
		else
		{
			if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
				UE_LOG(SFCollectable, Error, TEXT("Collectable item has invalid item tag {%s}"), *ItemTag.ToString());
			if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collectable item has invalid item tag {" + ItemTag.ToString() + "}");

		}
	}

	Cast<ASf_FP_Character>(TriggeringPawn)->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(PickupMontage);

	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
		UE_LOG(SFCollectable, Log, TEXT("Collecting Collectable."));
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collecting Collectable.");

	OnCollect(InteractComponent->GetLastHitResult().Location, TriggeringPawn);
	bHasBeenCollected = true;
	
	if (AutoDestroyOnPickup)
		Destroy();

	
}

void ACollectable::OnCollect_Implementation(FVector CollectLocation, APawn* TriggeringPawn)
{
}