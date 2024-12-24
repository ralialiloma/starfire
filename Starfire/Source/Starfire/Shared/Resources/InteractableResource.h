#pragma once

#include "CoreMinimal.h"
#include "Starfire/Shared/Interact/InteractInterfaces.h"
#include "Resource.h"
#include "InteractableResource.generated.h"

UCLASS(Blueprintable, Abstract)
class STARFIRE_API AInteractableResource : public AResource, public IPrimaryInteract, public IHighlightInterface
{
	GENERATED_BODY()
	
public:

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn) override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSettings")
	UAnimMontage* PickupMontage = nullptr;
	
};
