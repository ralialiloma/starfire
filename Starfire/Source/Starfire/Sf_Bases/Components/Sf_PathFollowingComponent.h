#pragma once
#include "Navigation/CrowdFollowingComponent.h"
#include "Sf_PathFollowingComponent.generated.h"

UCLASS()
class USf_PathFollowingComponent: public UCrowdFollowingComponent
{
	GENERATED_BODY()
public:
	virtual void FollowPathSegment(float DeltaTime) override;

protected:
	FVector LastTargetLocation;
	
};
