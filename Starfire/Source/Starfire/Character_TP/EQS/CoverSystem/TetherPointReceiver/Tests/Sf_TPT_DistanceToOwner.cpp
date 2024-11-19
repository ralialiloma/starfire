#include "Sf_TPT_DistanceToOwner.h"

#include "NavigationSystem.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Utility/AsyncUtility.h"


TArray<TWeakObjectPtr<UTetherPoint>> USf_TPT_DistanceToOwner::GetTetherPointsBlocking(TWeakObjectPtr<AActor> OwningActor,
                                                                                      TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
                                                                                      TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints)
{
	
	TArray<TWeakObjectPtr<UTetherPoint>>  ReturnTetherPoints{};
	TMap<TWeakObjectPtr<UTetherPoint>, double> TetherPointLengths{};
	FVector ActorLocation = OwningActor->GetActorLocation();

	for (TWeakObjectPtr<UTetherPoint> TetherPoint: TetherPoints)
	{
		if (!TetherPoint.IsValid())
			continue;

		FVector TetherPointLocation = TetherPoint->CenterLocation;

		//Verify Squared Distance
		const double DistanceSquared = FVector::DistSquared(ActorLocation, TetherPointLocation);
		if (DistanceSquared < FMath::Square(MinDistance) || DistanceSquared > FMath::Square(MaxDistance))
			continue;

		TWeakObjectPtr<USf_TPT_DistanceToOwner> WeakSelf = this;
		TFuture<void> GetPathFuture =  FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf, ActorLocation,TetherPointLocation]()->void
			{
				UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(WeakSelf->GetWorld());
				FPathFindingQuery PathFindingQuery = FPathFindingQuery(WeakSelf.Get(), *NavSystem->GetDefaultNavDataInstance(),ActorLocation,
				TetherPointLocation);
				WeakSelf->bFinishedNavQuery = false;
				FNavPathQueryDelegate PathQueryDelegate;

				const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
				PathQueryDelegate.BindUObject(WeakSelf.Get(), &USf_TPT_DistanceToOwner::OnNavPathQueryFinish);
				NavSystem->FindPathAsync( NavSystem->GetDefaultSupportedAgent(),PathFindingQuery, PathQueryDelegate);
			});
		

		FAsyncUtility::Wait<void>(GetPathFuture,WeakSelf);
		while (!bFinishedNavQuery)
		{
			FAsyncUtility::WaitForSeconds(0.01f,WeakSelf);
		}
	
		//Verify Path Validity
		TFuture<bool> PathValidFuture  =  FAsyncUtility::RunOnGameThread<bool>(WeakSelf,[WeakSelf]()->bool
			{
				if (!WeakSelf.IsValid())
					return false;
			
				if (!WeakSelf->NavPathShared.IsValid())
					return false;
			
				return WeakSelf->NavPathShared.Get()->IsValid() && !WeakSelf->NavPathShared.Get()->IsPartial();
			});
		FAsyncUtility::Wait(PathValidFuture,WeakSelf);
		bool bIsPathValid =  FAsyncUtility::GetFutureValue<bool>(MoveTemp(PathValidFuture));
		if(!bIsPathValid)
			continue;
		
		//Verify Path Length
		TFuture<double> PathLengthFuture  =  FAsyncUtility::RunOnGameThread<double>(WeakSelf,[WeakSelf]()->double
			{
				if (!WeakSelf.IsValid())
					return 0;
				
				if (!WeakSelf->NavPathShared.IsValid())
					return 0;
			
				return WeakSelf->NavPathShared.Get()->GetLength();
			});
		FAsyncUtility::Wait(PathLengthFuture,WeakSelf);
		const double PathLength = FAsyncUtility::GetFutureValue<double>(MoveTemp(PathLengthFuture));
		
		if (PathLength<MinDistance||PathLength>MaxDistance||PathLength<0.1f)
			continue;
		
		ReturnTetherPoints.Add(TetherPoint);
		TetherPointLengths.Add(TetherPoint,PathLength);
	}

	ReturnTetherPoints.Sort([&TetherPointLengths](const TWeakObjectPtr<UTetherPoint>& A, const TWeakObjectPtr<UTetherPoint>& B)
		{
			return TetherPointLengths[A] < TetherPointLengths[B];
		}
	);
	
	return ReturnTetherPoints;
}

void USf_TPT_DistanceToOwner::OnNavPathQueryFinish(uint32, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr InNavPathShared)
{
	NavPathShared = InNavPathShared;
	bFinishedNavQuery = true;
}
