#pragma once
#include "Navigation/PathFollowingComponent.h"
#include "Sf_PathFollowingComponent.generated.h"


UCLASS(BlueprintType)
class USf_PathFollowingComponent: public UPathFollowingComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	FVector GetCurrentDirection_BP() const;

	UFUNCTION(BlueprintCallable)
	FVector GetCurrentMoveInput_BP() const;
};
