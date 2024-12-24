#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Resource.generated.h"

class UInventoryComponent;
class AResourceSpawner;
DECLARE_LOG_CATEGORY_EXTERN(SFCollectable, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollect_BP, AResource*, Resource);
DECLARE_MULTICAST_DELEGATE_OneParam(FCollect_CPP, AResource*);

UCLASS(Blueprintable, Abstract)
class STARFIRE_API AResource : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCollect_BP OnCollectDelegate_BP;
	FCollect_CPP OnCollectDelegate_CPP;
	
	AResource();

	UFUNCTION(BlueprintNativeEvent)
	void OnCollect(FVector CollectLocation, APawn* TriggeringPawn);

	UFUNCTION(BlueprintCallable)
	virtual UStaticMesh* GetStaticMeshAsset() const;
	UFUNCTION(BlueprintCallable)
	virtual UStaticMeshComponent* GetStaticMeshComp() const;
	UFUNCTION(BlueprintCallable)
	virtual FVector GetMeshScaling() const;
	UFUNCTION(BlueprintCallable)
	virtual FGameplayTag GetItemTag() const;
	
	UFUNCTION(BlueprintCallable)
	uint8 GetResourceVeinGroup();
	
protected:

	friend AResourceSpawner;

	UFUNCTION(BlueprintCallable)
	bool AddSelfToInventory(UInventoryComponent* Inventory);
	
	UFUNCTION(BlueprintCallable)
	void AssignResourceVein(uint8 VeinID);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	int CollectAmount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	bool bAutoDestroyOnCollect = true;
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
