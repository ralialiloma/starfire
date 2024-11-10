#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/StarFireGameplayTags.h"
#include "ResourceSpawnLocation.generated.h"

class AResource;

UCLASS(Blueprintable, Abstract)
class STARFIRE_API AResourceSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:
	AResourceSpawnLocation();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const;

protected:
	
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ItemTag = Sf_GameplayTags::Gameplay::Resource::Environment;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<AResource> DisplayResourceClass;
#endif

};
