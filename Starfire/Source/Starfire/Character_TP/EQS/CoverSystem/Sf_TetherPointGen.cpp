﻿
#include "Sf_TetherPointGen.h"

#include "CollisionDebugDrawingPublic.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


UTetherPoint::UTetherPoint(): CenterLocation(FVector::ZeroVector),
                              VerticalOffset(0.0f),
                              Score(0.0f),
                              Scale(1.0f),
                              Size(FVector::ZeroVector)
{
}

bool UTetherPoint::operator==(const UTetherPoint& Other) const
{
	return CenterLocation.Equals(Other.CenterLocation) &&
		FMath::IsNearlyEqual(VerticalOffset, Other.VerticalOffset) &&
		Score == Other.Score &&
		Scale == Other.Scale &&
		Size == Other.Size &&
		SurroundingPoints == Other.SurroundingPoints;
}

bool UTetherPoint::operator!=(const UTetherPoint& Other) const
{
	return !(*this == Other);
}

void UTetherPoint::FindSurroundingPoints(const UObject* WorldContextObject)
{
	for (int32 x = 0; x <= Size.X; x++)
	{
		for (int32 y = 0; y <= Size.Y; y++)
		{
			for (int32 z = 0; z <= Size.Z; z++)
			{
				FVector Point = CenterLocation + FVector::UpVector*VerticalOffset + FVector(x, y, z) * Scale;
				
				TArray<TEnumAsByte<EObjectTypeQuery>>  ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>> {EObjectTypeQuery::ObjectTypeQuery_MAX};
				TArray<AActor*> OverlappedActors;
				UKismetSystemLibrary::SphereOverlapActors(
					WorldContextObject,
					Point,
					Scale/2,
					ObjectTypes,
					AActor::StaticClass(),
					TArray<AActor*>{},
					OverlappedActors);

				if (OverlappedActors.Num()>0)
					continue;
				
				SurroundingPoints.Add(Point);
			}
		}
	}
}

void UTetherPoint::UpdateScore(const FVector& PlayerLocation, const UObject* WorldContextObject, const TArray<AActor*>& IgnoredActors, ETraceTypeQuery TraceTypeQuery)
{
	const int NumSurroundingPoints = SurroundingPoints.Num();
	if (NumSurroundingPoints<=0)
	{
		Score = 0;
		return;
	}
		
	int CoveredPoints = 0;
	for (const FVector Point: SurroundingPoints)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(
			WorldContextObject,
			Point,
			PlayerLocation,
			TraceTypeQuery,
			false,
			IgnoredActors,
			EDrawDebugTrace::Type::None,
			HitResult,
			true,
			FLinearColor::Blue,
			FLinearColor::Red,
			1.0f);

		if (HitResult.bBlockingHit)
		{
			CoveredPoints++;
		}
		DistanceToWall =  HitResult.Distance;
	}
	
	Score = static_cast<float>(CoveredPoints)/ static_cast<float>(NumSurroundingPoints) ;
}

ASf_TetherPointGen::ASf_TetherPointGen(): TraceTypeQuery()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASf_TetherPointGen::BeginPlay()
{
	Super::BeginPlay();
	GeneratePoints();
	
	GetWorld()->GetTimerManager().SetTimer(CloseToPlayerTethers,[this]()->void{AddCloseToPlayerTetherPointsToProcess();},PlayerUpdateRateInSeconds,true);
	//GetWorld()->GetTimerManager().SetTimer(OtherTethers,[this]()->void{AddOtherTetherPointsToProcess();},OtherUpdateRateInSecdonds,true);
}

void ASf_TetherPointGen::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTetherPoints();
}

void ASf_TetherPointGen::GeneratePoints()
{
	for (int32 x = -Size.X / 2; x <= Size.X / 2; x++)
	{
		for (int32 y = -Size.Y / 2; y <= Size.Y / 2; y++)
		{
			for (int32 z = -Size.Z / 2; z <= Size.Z / 2; z++)
			{
				FVector CenterLocation = GetActorLocation()+ FVector(x, y, z) * Scale;

				FNavLocation ProjectedNavCenterLocation;
				FVector ProjectExtent = FVector(Scale/2.0f);
				if (!UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(CenterLocation, ProjectedNavCenterLocation, ProjectExtent))
					continue;
				
				UTetherPoint* TetherPoint = NewObject<UTetherPoint>(this);
				TetherPoint->Score = 0;
				TetherPoint->CenterLocation = ProjectedNavCenterLocation.Location;
				TetherPoint->Scale =   TetherScale;
				TetherPoint->Size = TetherPointSize;
				TetherPoint->VerticalOffset = VerticalOffset;
				TetherPoint->FindSurroundingPoints(this);
				AllTetherPoints.Add(TetherPoint);
			}
		}
	}
}



TArray<FVector> ASf_TetherPointGen::GetCoverLocations(float MinScore) const
{
	TArray<FVector> CoverLocations{};
	for (const UTetherPoint* Point: AllTetherPoints)
	{
		if (Point->Score>MinScore &&  Point->DistanceToWall < MaxWallDistance)
		{
			CoverLocations.Add(Point->CenterLocation);
		}
	}
	return CoverLocations;
}

TArray<FVector> ASf_TetherPointGen::GetPeakLocations(const float MaxScore) const
{
	TArray<FVector> CoverLocations{};
	for (const UTetherPoint* Point: AllTetherPoints)
	{
		if (Point->Score<MaxScore)
		{
			CoverLocations.Add(Point->CenterLocation);
		}
	}
	return CoverLocations;
}

void ASf_TetherPointGen::UpdateTetherPoints()
{
	if (TetherPointsToProcess.Num()<=0)
		return;
	
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const TArray<AActor*> IgnoredActors = TArray<AActor*>{this,USf_FunctionLibrary::GetSfPlayerpawn(this)};

	int AmountOfTetherPointsToProcess = TetherPointsToProcess.Num();
	TArray<UTetherPoint*> LocalTetherPointsToProcess = TetherPointsToProcess;
	for (int i= 0; i<FMath::Min(AmountOfTetherPointsToProcess,AmountOfUpdatedTetherPointsPerFrame);i++)
	{
		LocalTetherPointsToProcess[i]->UpdateScore(PlayerLocation, this, IgnoredActors, TraceTypeQuery);
		TetherPointsToProcess.Remove(LocalTetherPointsToProcess[i]);
		
			if (LocalTetherPointsToProcess[i]->Score>0.5f &&  LocalTetherPointsToProcess[i]->DistanceToWall < MaxWallDistance)
			{
				DrawDebugSphere(GetWorld(), LocalTetherPointsToProcess[i]->CenterLocation, Scale / 2.0f, 6, FColor::Green, false,1);
			}
			else
			{
				DrawDebugSphere(GetWorld(), LocalTetherPointsToProcess[i]->CenterLocation, Scale / 2.0f, 6, FColor::Red, false,1);
			}
	}
	
}


void ASf_TetherPointGen::AddCloseToPlayerTetherPointsToProcess()
{
	const TArray<UTetherPoint*> TetherPointsAroundPlayer  = GetTetherPointsAroundPlayer();
	for(UTetherPoint* TetherPoint: TetherPointsAroundPlayer)
	{
		TetherPointsToProcess.AddUnique(TetherPoint);
	}
}

TArray<UTetherPoint*> ASf_TetherPointGen::GetTetherPointsAroundPlayer() const
{
	FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);

	TArray<UTetherPoint*> TetherPointsAroundPlayer{};
	for (UTetherPoint* TetherPoint: AllTetherPoints)
	{
		const float Distance = FVector::Dist(PlayerLocation, TetherPoint->CenterLocation);
		if (MinUpdateDistance < Distance && Distance < MaxUpdateDistance)
		{
			TetherPointsAroundPlayer.Add(TetherPoint);
		}

	}
	return TetherPointsAroundPlayer;
}