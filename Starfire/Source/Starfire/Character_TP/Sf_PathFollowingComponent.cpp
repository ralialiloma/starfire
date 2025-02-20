#include "Sf_PathFollowingComponent.h"


FVector USf_PathFollowingComponent::GetCurrentDirection_BP() const
{
	return  GetCurrentDirection();
}

FVector USf_PathFollowingComponent::GetCurrentMoveInput_BP() const
{
	return GetCurrentMoveInput();
}
