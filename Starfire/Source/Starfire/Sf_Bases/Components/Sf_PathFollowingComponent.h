#pragma once
#include "Navigation/CrowdFollowingComponent.h"
#include "Sf_PathFollowingComponent.generated.h"

UCLASS()
class USf_PathFollowingComponent: public UCrowdFollowingComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void FollowPathSegment(float DeltaTime) override;

protected:
	UPROPERTY()
	FVector LastTargetLocation;
	UPROPERTY()
	AActor* OwningPawn;
	
};
