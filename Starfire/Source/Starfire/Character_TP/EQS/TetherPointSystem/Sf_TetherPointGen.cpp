
#include "Sf_TetherPointGen.h"
#include "NavigationSystem.h"
#include "Sf_TetherPointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolAreaManager.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


UTetherPoint::UTetherPoint(): CenterLocation(FVector::ZeroVector),
                              VerticalOffset(0.0f),
                              CoverPotential(0.0f),
                              Scale(1.0f),
                              Size(FVector::ZeroVector)
{
}

bool UTetherPoint::operator==(const UTetherPoint& Other) const
{
	return CenterLocation.Equals(Other.CenterLocation) &&
		FMath::IsNearlyEqual(VerticalOffset, Other.VerticalOffset) &&
		CoverPotential == Other.CoverPotential &&
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

				FHitResult HitResult;
				UKismetSystemLibrary::LineTraceSingle(
					WorldContextObject,
					Point + FVector::UpVector * Scale, // A little above the point
					Point - FVector::UpVector * Scale, // A little below the point
					ETraceTypeQuery::TraceTypeQuery_MAX, // Replace with your desired trace type
					false,
					TArray<AActor*>{},
					EDrawDebugTrace::None,
					HitResult,
					true,
					FLinearColor::Blue,
					FLinearColor::Red,
					2.0f
				);
				
				/*TArray<TEnumAsByte<EObjectTypeQuery>>  ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>> {EObjectTypeQuery::ObjectTypeQuery_MAX};
				TArray<AActor*> OverlappedActors;
				UKismetSystemLibrary::SphereOverlapActors(
					WorldContextObject,
					Point,
					Scale/2,
					ObjectTypes,
					AActor::StaticClass(),
					TArray<AActor*>{},
					OverlappedActors);*/

				if (HitResult.bBlockingHit)
				{
					continue;
				}

				//if (OverlappedActors.Num()>0)
				//	continue;
				
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
		CoverPotential = 0;
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
			2.0f);

		if (HitResult.bBlockingHit)
		{
			CoveredPoints++;
		}
		DistanceToWall =  HitResult.Distance;
	}
	
	CoverPotential = static_cast<float>(CoveredPoints)/ static_cast<float>(NumSurroundingPoints) ;
}

ASf_TetherPointGen* ASf_TetherPointGen::Get(const UObject* WorldContextObject)
{
	TArray<AActor*> Actors{};
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject,ASf_TetherPointGen::StaticClass(),Actors);
	if (Actors.Num()<=0)
		return  nullptr;
	return Cast<ASf_TetherPointGen>(Actors[0]);
}

ASf_TetherPointGen::ASf_TetherPointGen(): TraceTypeQuery()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASf_TetherPointGen::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<USf_TetherPointSubsystem>()->RegisterTetherPointGen(this);
	GeneratePoints();
	
	GetWorld()->GetTimerManager().SetTimer(CloseToPlayerTethers,[this]()->void{AddRelevantTetherPointsToProcess();},PlayerUpdateRateInSeconds,true);
	//GetWorld()->GetTimerManager().SetTimer(OtherTethers,[this]()->void{AddOtherTetherPointsToProcess();},OtherUpdateRateInSecdonds,true);
}

void ASf_TetherPointGen::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTetherPoints();
}

void ASf_TetherPointGen::GeneratePoints()
{
	UNavigationSystemV1* NavSystem =  UNavigationSystemV1::GetCurrent(GetWorld());
	FVector ProjectExtent = FVector(Scale/2.0f);
	const ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	
	const FSharedConstNavQueryFilter QueryFilter = NavData->GetQueryFilter(QueryFilterClass);
	const FNavAgentProperties NavAgentProperties =  FNavAgentProperties::DefaultProperties;
	
	for (int32 x = -Size.X / 2; x <= Size.X / 2; x++)
	{
		for (int32 y = -Size.Y / 2; y <= Size.Y / 2; y++)
		{
			for (int32 z = -Size.Z / 2; z <= Size.Z / 2; z++)
			{
				FVector CenterLocation = GetActorLocation()+ FVector(x, y, z) * Scale;

				if (!USf_PatrolAreaManager::Get(GetWorld())->IsPointInAnyPatrolArea(CenterLocation))
					continue;
				
				FNavLocation ProjectedNavCenterLocation;
				if (!NavSystem->ProjectPointToNavigation(CenterLocation, ProjectedNavCenterLocation, ProjectExtent,&NavAgentProperties,QueryFilter))
					continue;
				
				UTetherPoint* TetherPoint = NewObject<UTetherPoint>(this);
				TetherPoint->CoverPotential = 0;
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



TArray<FVector> ASf_TetherPointGen::GetCoverLocations(const float MinScore) const
{
	TArray<FVector> CoverLocations{};
	for (const UTetherPoint* Point: AllTetherPoints)
	{
		if (Point->CoverPotential>MinScore &&  Point->DistanceToWall < MaxWallDistance)
		{
			CoverLocations.Add(Point->CenterLocation);
		}
	}
	return CoverLocations;
}

TArray<UTetherPoint*> ASf_TetherPointGen::GetAllTetherPoints()
{
	return  AllTetherPoints;
}


bool ASf_TetherPointGen::VerifyCover(const FVector LocationToVerify, const float Extent, const float MinScore) const
{
	float Distance;
	FVector FoundCoverLoc;
	bool bFoundLocation;
	GetClosestCoverTo(LocationToVerify,MinScore,Distance,FoundCoverLoc,bFoundLocation);

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(Distance));
	
	if (!bFoundLocation)
		return false;

	if (Distance<=Extent)
		return true;

	return false;
}

bool ASf_TetherPointGen::VerifyPeak(const FVector LocationToVerify, const float Extent, const float MaxScore) const
{
	float Distance;
	FVector FoundCoverLoc;
	bool bFoundLocation;
	GetClosestPeakTo(LocationToVerify,MaxScore,Distance,FoundCoverLoc,bFoundLocation);
	
	if (!bFoundLocation)
		return false;

	if (Distance<=Extent)
		return true;

	return false;
}

void ASf_TetherPointGen::GetClosestCoverTo(
	const FVector Location,
	const float MinScore,
	float& OutDistance,
	FVector& ClosestCoverLocation,
	bool& bFound) const
{
	OutDistance = 0;
	ClosestCoverLocation = FVector::ZeroVector;
	bFound = false;

	TArray<FVector> CoverLocations =  GetCoverLocations(MinScore);
	if (CoverLocations.Num()<=0)
		return;

	CoverLocations.Sort([Location](const FVector& A, const FVector& B)
	{
		return FVector::DistSquared(Location, A) < FVector::DistSquared(Location, B);
	});


	OutDistance =  FVector::Dist(Location, CoverLocations[0]);
	ClosestCoverLocation = CoverLocations[0];
	bFound = true;
}

void ASf_TetherPointGen::GetClosestPeakTo(
	const FVector Location,
	const float MaxScore,
	float& OutDistance,
	FVector& ClosestCoverLocation,
	bool& bFound) const
{
	OutDistance = 0;
	ClosestCoverLocation = FVector::ZeroVector;
	bFound = false;

	TArray<FVector> CoverLocations =  GetPeakLocationsInRange(MaxScore);
	if (CoverLocations.Num()<=0)
		return;

	CoverLocations.Sort([Location](const FVector& A, const FVector& B)
	{
		return FVector::DistSquared(Location, A) < FVector::DistSquared(Location, B);
	});


	OutDistance =  FVector::Dist(Location, CoverLocations[0]);
	ClosestCoverLocation = CoverLocations[0];
	bFound = true;
}

TArray<FVector> ASf_TetherPointGen::GetPeakLocationsInRadius(const float MaxScore, const FVector& Location, float Radius) const
{
	TArray<FVector> CoverLocations{};
	for (const UTetherPoint* Point: AllTetherPoints)
	{
		if (Point->CoverPotential<MaxScore &&  FVector::Distance(Location, Point->CenterLocation)<=Radius)
		{
			CoverLocations.Add(Point->CenterLocation);
		}
	}
	return CoverLocations;
}

TArray<FVector> ASf_TetherPointGen::GetPeakLocationsInRange(const float MaxScore) const
{
	TArray<FVector> CoverLocations{};
	for (const UTetherPoint* Point: AllTetherPoints)
	{
		if (Point->CoverPotential<MaxScore)
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

	TArray<AActor*> IgnoredActors = TArray<AActor*>{this,USf_FunctionLibrary::GetSfPlayerpawn(this)};
	TArray<AActor*> TPCharacters{};
	UGameplayStatics::GetAllActorsOfClass(this,ASf_TP_Character::StaticClass(),TPCharacters);
	IgnoredActors.Append(TPCharacters);

	int AmountOfTetherPointsToProcess = TetherPointsToProcess.Num();
	TArray<UTetherPoint*> LocalTetherPointsToProcess = TetherPointsToProcess;
	for (int i= 0; i<FMath::Min(AmountOfTetherPointsToProcess,AmountOfUpdatedTetherPointsPerFrame);i++)
	{
		LocalTetherPointsToProcess[i]->UpdateScore(PlayerLocation, this, IgnoredActors, TraceTypeQuery);
		TetherPointsToProcess.Remove(LocalTetherPointsToProcess[i]);

		if (SHOULD_DEBUG(TP::EQS::TetherPointGen,EDebugType::Visual))
		{
			FLinearColor InterpolatedColor = FLinearColor::LerpUsingHSV(FColor::Red, FColor::Green, LocalTetherPointsToProcess[i]->CoverPotential);
			FColor DrawColor = InterpolatedColor.ToFColor(true);
			
			if (LocalTetherPointsToProcess[i]->DistanceToWall < MaxWallDistance)
			{
				DrawDebugSphere(GetWorld(), LocalTetherPointsToProcess[i]->CenterLocation, Scale / 2.0f, 6, DrawColor, false,1);
			}
		}
			
	}
	
}


void ASf_TetherPointGen::AddRelevantTetherPointsToProcess()
{
	const TArray<UTetherPoint*> TetherPointsAroundPlayer  = GetRelevantTetherPoints();
	for(UTetherPoint* TetherPoint: TetherPointsAroundPlayer)
	{
		TetherPointsToProcess.AddUnique(TetherPoint);
	}
}

TArray<UTetherPoint*> ASf_TetherPointGen::GetRelevantTetherPoints() const
{
	TArray<UTetherPoint*> RelevantPoints{};
	TArray<AActor*> RelevantActors = GetRelevantActors();
	
	for (UTetherPoint* TetherPoint: AllTetherPoints)
	{
		for (const AActor* Actor: RelevantActors)
		{
			const float ActorToTetherPoint = FVector::Dist(Actor->GetActorLocation(), TetherPoint->CenterLocation);
			if (MinActorUpdateDistance < ActorToTetherPoint && ActorToTetherPoint < MaxActorUpdateDistance)
			{
				RelevantPoints.AddUnique(TetherPoint);
				break;
			}
		}
	}
	return RelevantPoints;
}

TArray<AActor*> ASf_TetherPointGen::GetRelevantActors() const
{
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	TArray<AActor*> Actors{};
	for (AActor* Actor: RegisteredActors)
	{
		if (!IsValid(Actor))
			continue;
			
		const float PlayerToActor = FVector::Dist(PlayerLocation, Actor->GetActorLocation());

		if (PlayerToActor<=MaxRelevancyDistance)
			Actors.AddUnique(Actor);
	}
	return Actors;
}

void ASf_TetherPointGen::RegisterActor(AActor* Actor)
{
	if (IsValid(Actor) && !RegisteredActors.Contains(Actor))
		RegisteredActors.AddUnique(Actor);
}

void ASf_TetherPointGen::UnregisterActor(AActor* Actor)
{
	if (IsValid(Actor))
		RegisteredActors.Remove(Actor);
}
