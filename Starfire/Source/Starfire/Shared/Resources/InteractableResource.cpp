#include "InteractableResource.h"
#include "Starfire/Character_FP/Components/Inventory/InventoryComponent.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"

void AInteractableResource::OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
{
	if (bHasBeenCollected)
		return;
	
	IPrimaryInteract::OnInteractStart_Implementation(InteractComponent, TriggeringPawn);

	if (!ItemTag.IsValid())
	{
		if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
			UE_LOG(SFCollectable, Error, TEXT("Collectable item has invalid item tag {%s}"), *ItemTag.ToString());
		if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collectable item has invalid item tag {" + ItemTag.ToString() + "}");
	}

	UInventoryComponent* Inventory = TriggeringPawn->GetComponentByClass<UInventoryComponent>();
	if (!Inventory)
		return;
	
	int RemainingCollections = Inventory->AddResource(ItemTag, CollectAmount);
	if (RemainingCollections == CollectAmount)
		return;

	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
		UE_LOG(SFCollectable, Log, TEXT("Collecting Collectable."));
	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collecting Collectable.");


	if (RemainingCollections >= 1)
	{
		CollectAmount = RemainingCollections;
		return;
	}
	
	OnCollect(InteractComponent->GetLastHitResult().Location, TriggeringPawn);
}