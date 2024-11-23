#include "Sf_PathFollowingComponent.h"

#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

void USf_PathFollowingComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	AController* OwningController = Cast<AController>(Owner);
	if (!IsValid(OwningController))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Component only allowed on controllers!");
		return;
	}

	OwningPawn = OwningController->GetPawn();
	
}

void USf_PathFollowingComponent::FollowPathSegment(const float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);

	if (!IsValid(OwningPawn))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid Owning Pawn!");
		return;
	}
	
	//Reserve Covers to prevent other NPCs from pursuing the same goal
	const FVector CurrentTargetLocation = GetCurrentTargetLocation();
	if (CurrentTargetLocation.Equals(LastTargetLocation))
		return;
	
	UNavigationTargetSubsystem* NavigationTargetSubsystem = UNavigationTargetSubsystem::Get(GetWorld());
	NavigationTargetSubsystem->UnregisterNavTarget(OwningPawn);
	NavigationTargetSubsystem->RegisterNavTarget(OwningPawn,CurrentTargetLocation);
	LastTargetLocation = CurrentTargetLocation;
}
