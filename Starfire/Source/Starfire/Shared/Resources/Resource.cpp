#include "Resource.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Character_FP/Components/Inventory/InventoryComponent.h"
#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY(SFCollectable);

AResource::AResource()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AResource::OnCollect_Implementation(FVector CollectLocation, APawn* TriggeringPawn)
{
	OnCollectDelegate_BP.Broadcast(this);
	OnCollectDelegate_CPP.Broadcast(this);

	bool Collected = false;
	if (TriggeringPawn)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (PlayerPawn == TriggeringPawn)
			Collected = AddSelfToInventory(PlayerPawn->GetComponentByClass<UInventoryComponent>());
	}
	
	bHasBeenCollected = Collected;

	UFXSubsystem* FXSubsystem = UFXSubsystem::Get();
	FXSubsystem->PlayFXAt(FXCollectMessage, GetActorTransform());

	if (bAutoDestroyOnCollect)
		Destroy();
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

bool AResource::AddSelfToInventory(UInventoryComponent* Inventory)
{
	if (!Inventory)
		return false;
	
	int RemainingCollections = Inventory->AddResource(ItemTag, CollectAmount);
	
	if (RemainingCollections >= 1)
		return false;

	return true;
}
