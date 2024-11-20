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
	
	UFUNCTION(BlueprintCallable)
	uint8 GetVeinGroup() const;

protected:
	
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ItemTag = Sf_GameplayTags::Gameplay::Resource::Environment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, UIMax = 20))
	uint8 VeinGroup = 0;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftClassPtr<AResource> DisplayResourceClass;
#endif

};
