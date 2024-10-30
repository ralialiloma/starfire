#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "Collectable.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SFCollectable, Log, All);

UCLASS(Blueprintable)
class STARFIRE_API ACollectable : public AActor, public IPrimaryInteract, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	
	ACollectable();

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnCollect(FVector CollectLocation, APawn* TriggeringPawn);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	int CollectAmount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	bool AutoDestroyOnPickup = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	UAnimMontage* PickupMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings", meta = (Categories="Gameplay.ItemType"))
	FGameplayTag ItemTag;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	bool bHasBeenCollected = false;
	
};
