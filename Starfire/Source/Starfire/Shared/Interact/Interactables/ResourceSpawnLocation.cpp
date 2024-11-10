#include "ResourceSpawnLocation.h"

#include "Resource.h"

AResourceSpawnLocation::AResourceSpawnLocation()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DisplayMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetHiddenInGame(true);
}

void AResourceSpawnLocation::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	if (DisplayResourceClass.LoadSynchronous())
	{
		UStaticMesh* Mesh = DisplayResourceClass->GetDefaultObject<AResource>()->GetStaticMeshAsset();
		FVector Scale = DisplayResourceClass->GetDefaultObject<AResource>()->GetMeshScaling();
		MeshComponent->SetStaticMesh(Mesh);
		MeshComponent->SetRelativeScale3D(Scale);
	}
#endif
	
}

FGameplayTag AResourceSpawnLocation::GetItemTag() const
{
	return ItemTag;
}
