#include "Sf_TPT_DistanceToPlayer.h"
#include "NavigationSystem.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


TArray<TWeakObjectPtr<UTetherPoint>> USf_TPT_DistanceToPlayer::GetTetherPointsBlocking(TWeakObjectPtr<AActor> OwningActor,
                                                                                      TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
                                                                                      TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints)
{
	
	TArray<TWeakObjectPtr<UTetherPoint>>  ReturnTetherPoints{};
	TMap<TWeakObjectPtr<UTetherPoint>, double> TetherPointLengths{};
	TWeakObjectPtr<USf_TPT_DistanceToPlayer> WeakSelf = this;

	TFuture<FVector>  PlayerLocationRetrieve =
		FAsyncUtility::RunOnGameThread<FVector>(
			WeakSelf,[WeakSelf]()->FVector
				{
					if (!WeakSelf.IsValid())
						return FVector::Zero();
						
					 return USf_FunctionLibrary::GetPlayerLocation(WeakSelf.Get());
				}
			);
	FVector ActorLocation = FAsyncUtility::GetFutureValue<FVector>(MoveTemp(PlayerLocationRetrieve));

	for (TWeakObjectPtr<UTetherPoint> TetherPoint: TetherPoints)
	{
		if (!TetherPoint.IsValid())
			continue;

		FVector TetherPointLocation = TetherPoint->CenterLocation;

		//Verify Squared Distance
		const double Distance = FVector::Distance(ActorLocation, TetherPointLocation);

		FString DistancePrint = FString::Printf(TEXT("Distance: %f, Min: %f, Max %f"),Distance,MinDistance,MaxDistance);
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::FromInt(TetherPoints.Num()));
		if (Distance < FMath::Square(MinDistance) || Distance > FMath::Square(MaxDistance))
			continue;
		
		TFuture<void> GetPathFuture =  FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf, ActorLocation,TetherPointLocation]()->void
			{
				UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(WeakSelf->GetWorld());
				FPathFindingQuery PathFindingQuery = FPathFindingQuery(
					WeakSelf.Get(),
					*NavSystem->GetDefaultNavDataInstance(),
					ActorLocation,
					TetherPointLocation);
				WeakSelf->bFinishedNavQuery = false;
				FNavPathQueryDelegate PathQueryDelegate;
				PathQueryDelegate.BindUObject(WeakSelf.Get(), &USf_TPT_DistanceToPlayer::OnNavPathQueryFinish);
				NavSystem->FindPathAsync(NavSystem->GetDefaultSupportedAgent(),PathFindingQuery, PathQueryDelegate);
			});
		
		FAsyncUtility::Wait<void>(GetPathFuture,WeakSelf);
		while (!bFinishedNavQuery)
			FAsyncUtility::WaitForSeconds(0.01f,WeakSelf);
	
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

void USf_TPT_DistanceToPlayer::OnNavPathQueryFinish(uint32, ENavigationQueryResult::Type ResultType, FNavPathSharedPtr InNavPathShared)
{
	NavPathShared = InNavPathShared;
	bFinishedNavQuery = true;
}
