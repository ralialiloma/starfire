#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "Resource.generated.h"

class UResourceSpawner;
DECLARE_LOG_CATEGORY_EXTERN(SFCollectable, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollect_BP, AResource*, Resource);
DECLARE_MULTICAST_DELEGATE_OneParam(FCollect_CPP, AResource*);

UCLASS(Blueprintable, Abstract)
class STARFIRE_API AResource : public AActor, public IPrimaryInteract, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCollect_BP OnCollectDelegate_BP;
	FCollect_CPP OnCollectDelegate_CPP;
	
	AResource();

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnCollect(FVector CollectLocation, APawn* TriggeringPawn);

	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetStaticMeshAsset() const;
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetStaticMeshComp() const;
	UFUNCTION(BlueprintCallable)
	FVector GetMeshScaling() const;
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const;
	
	UFUNCTION(BlueprintCallable)
	uint8 GetResourceVeinGroup();
	
protected:

	friend UResourceSpawner;

	UFUNCTION(BlueprintCallable)
	void AssignResourceVein(uint8 VeinID);

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bHasBeenCollected = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	uint8 VeinGroup;
	
};
