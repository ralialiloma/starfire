#include "Resource.h"

#include "GameFramework/Character.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Character_FP/Components/Inventory/InventoryComponent.h"
#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY(SFCollectable);

AResource::AResource()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AResource::OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
{
	if (bHasBeenCollected)
		return;
	
	IPrimaryInteract::OnInteractStart_Implementation(InteractComponent, TriggeringPawn);

	if (!ItemTag.IsValid())
	{
		if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
			UE_LOG(SFCollectable, Error, TEXT("Collectable item has invalid item tag {%s}"), *ItemTag.ToString());
		if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collectable item has invalid item tag {" + ItemTag.ToString() + "}");
	}

	UInventoryComponent* Inventory = TriggeringPawn->GetComponentByClass<UInventoryComponent>();
	if (!Inventory)
		return;
	
	int RemainingCollections = Inventory->AddResource(ItemTag, CollectAmount);
	if (RemainingCollections == CollectAmount)
		return;

	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Log))
		UE_LOG(SFCollectable, Log, TEXT("Collecting Collectable."));
	if (UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::Interactables::Name, EDebugType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Collecting Collectable.");


	if (RemainingCollections >= 1)
	{
		CollectAmount = RemainingCollections;
		return;
	}
	
	OnCollect(InteractComponent->GetLastHitResult().Location, TriggeringPawn);
	bHasBeenCollected = true;
	
	if (AutoDestroyOnPickup)
		Destroy();

	
}

void AResource::OnCollect_Implementation(FVector CollectLocation, APawn* TriggeringPawn)
{
	OnCollectDelegate_BP.Broadcast(this);
	OnCollectDelegate_CPP.Broadcast(this);
}

UStaticMesh* AResource::GetStaticMeshAsset() const
{
	return Mesh->GetStaticMesh();
}

UStaticMeshComponent* AResource::GetStaticMeshComp() const
{
	return Mesh;
}

FVector AResource::GetMeshScaling() const
{
	return Mesh->GetRelativeScale3D();
}

FGameplayTag AResource::GetItemTag() const
{
	return ItemTag;
}

void AResource::AssignResourceVein(uint8 VeinID)
{
	VeinGroup = VeinID;
}

uint8 AResource::GetResourceVeinGroup()
{
	return VeinGroup;
}
