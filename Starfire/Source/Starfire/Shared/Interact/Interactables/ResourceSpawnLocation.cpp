#include "ResourceSpawnLocation.h"

#include "EngineUtils.h"
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
		RootComponent->SetRelativeScale3D(Scale);

		if (VeinColor != VeinColorCache)
			RefreshAllVeinColors();
		
	}
#endif
	
}

FGameplayTag AResourceSpawnLocation::GetItemTag() const
{
	return ItemTag;
}

uint8 AResourceSpawnLocation::GetVeinGroup() const
{
	return VeinGroup;
}

#if WITH_EDITORONLY_DATA
void AResourceSpawnLocation::RefreshVeinColors()
{
	if (UMaterialInterface* Material = MeshComponent->GetMaterial(0))
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(FName("Base Color"), VeinColor);
		}
		else
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(DisplayResourceClass->GetDefaultObject<AResource>()->GetStaticMeshComp()->GetMaterial(0), this);
			DynamicMaterial->SetVectorParameterValue(FName("Base Color"), VeinColor);
			MeshComponent->SetMaterial(0, DynamicMaterial);
		}
	}
	VeinColorCache = VeinColor;
}

void AResourceSpawnLocation::SetVeinColor(FColor Color)
{
	UE_LOG(LogTemp, Log, TEXT("Setting to color: %s"), *Color.ToString())
	VeinColor = Color;
	RefreshVeinColors();
}

void AResourceSpawnLocation::RefreshAllVeinColors()
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	RefreshVeinColors();
	
	UClass* ActorClass = GetClass();
	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		AResourceSpawnLocation* OtherActor = Cast<AResourceSpawnLocation>(*It);
		if (OtherActor && OtherActor->VeinGroup == VeinGroup)
		{
			OtherActor->SetVeinColor(VeinColor);
		}
	}
}

void AResourceSpawnLocation::ForceRefreshMaterials()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	UClass* ActorClass = GetClass();
	
	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		AResourceSpawnLocation* OtherActor = Cast<AResourceSpawnLocation>(*It);
		if (OtherActor)
		{
			OtherActor->DynamicMaterial = nullptr;
			if (!OtherActor->MeshComponent->GetMaterial(0))
			{
				UE_LOG(LogTemp, Log, TEXT("Material id dumb"))
			}
		}
	}
}

void AResourceSpawnLocation::SelectVein() const
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	UClass* ActorClass = GetClass();
	
	for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
	{
		AResourceSpawnLocation* OtherActor = Cast<AResourceSpawnLocation>(*It);
		if (OtherActor && OtherActor->VeinGroup == VeinGroup)
		{
			GEditor->SelectActor(OtherActor, true, true);
		}
	}
}
#endif
