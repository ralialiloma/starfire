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
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const;
	
	UFUNCTION(BlueprintCallable)
	uint8 GetVeinGroup() const;

protected:
	
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	FGameplayTag ItemTag = Sf_GameplayTags::Gameplay::Resource::Environment;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, UIMax = 20), Category = "Settings")
	uint8 VeinGroup = 0;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION()
	void RefreshVeinColors();
	UFUNCTION()
	void SetVeinColorMaterial(FColor Color, UMaterialInstanceDynamic* Material);
	UFUNCTION()
	void RefreshAllVeinColors();
	
	UFUNCTION(CallInEditor, Category = "Settings")
	void SelectVein() const;
#endif

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	FColor VeinColor = FColor::White;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	TSoftClassPtr<AResource> DisplayResourceClass;

	//Editor Color Display
	UPROPERTY(Transient)
	FColor VeinColorCache;
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial;
#endif
	


};
