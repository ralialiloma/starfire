#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "Resource.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SFCollectable, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollect, AResource*, Resource);

UCLASS(Blueprintable, Abstract)
class STARFIRE_API AResource : public AActor, public IPrimaryInteract, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCollect OnCollectDelegate;
	
	AResource();

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnCollect(FVector CollectLocation, APawn* TriggeringPawn);

	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetStaticMeshAsset() const;
	UFUNCTION(BlueprintCallable)
	FVector GetMeshScaling() const;
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	int CollectAmount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	bool AutoDestroyOnPickup = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	UAnimMontage* PickupMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings", meta = (Categories="Gameplay.Resource"))
	FGameplayTag ItemTag;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	bool bHasBeenCollected = false;
	
};
