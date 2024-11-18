#include "Sf_TPT_DistanceToOwner.h"

#include "NavigationSystem.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"



TArray<UTetherPoint*> USf_TPT_DistanceToOwner::GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint, TArray<UTetherPoint*> TetherPoints)
{
	TArray<UTetherPoint*> ReturnTetherPoints{};

	FVector ActorLocation = OwningActor->GetActorLocation();
	for (UTetherPoint* TetherPoint: TetherPoints)
	{
		double DistanceSquared = FVector::DistSquared(ActorLocation, TetherPoint->CenterLocation);
		if (DistanceSquared < FMath::Square(MinDistance) || DistanceSquared > FMath::Square(MaxDistance))
		{
			continue;
		}
		
		double PathLength = 0;
		UNavigationSystemV1::GetCurrent(GetWorld())->GetPathLength(this,ActorLocation,TetherPoint->CenterLocation,PathLength);
		
		if (PathLength>MinDistance && PathLength<MaxDistance)
			ReturnTetherPoints.Add(TetherPoint);
	}
	return ReturnTetherPoints;
}
