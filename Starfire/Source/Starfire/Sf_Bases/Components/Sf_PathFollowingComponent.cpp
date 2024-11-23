#include "Sf_PathFollowingComponent.h"

#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"

void USf_PathFollowingComponent::FollowPathSegment(const float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);
	
	//Reserve Covers to prevent other NPCs from pursuing the same goal
	const FVector CurrentTargetLocation = GetCurrentTargetLocation();
	if (CurrentTargetLocation.Equals(LastTargetLocation))
		return;
	
	UNavigationTargetSubsystem* NavigationTargetSubsystem = UNavigationTargetSubsystem::Get(GetWorld());
	NavigationTargetSubsystem->RegisterNavTarget(CurrentTargetLocation);
	NavigationTargetSubsystem->UnregisterNavTarget(LastTargetLocation);
	LastTargetLocation = CurrentTargetLocation;
}
