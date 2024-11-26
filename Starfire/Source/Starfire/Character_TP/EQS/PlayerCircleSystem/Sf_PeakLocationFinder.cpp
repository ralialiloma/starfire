#include "Sf_PeakLocationFinder.h"

#include "NavigationSystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

USf_PeakLocationFinder::USf_PeakLocationFinder(const FObjectInitializer& ObjectInitializer)
{
}

void USf_PeakLocationFinder::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle,[this]()->void{Tick();},Tickrate,true);
}

void USf_PeakLocationFinder::Tick()
{
	GeneratedCircles.Empty();
	GeneratedCircles =
		GenerateCircleAroundPlayer(5,15,200.f,300.f, 500.f);
	DrawDebugCircles(GeneratedCircles,GetWorld(),FColor::Red,FColor::Green,50.f,Tickrate+0.01f);
	UpdateActorAssignments();
}

USf_PeakLocationFinder* USf_PeakLocationFinder::GetCurrent(const UWorld* World)
{
	if(!IsValid(World))
		return nullptr;
	
	return  World->GetGameInstance()->GetSubsystem<USf_PeakLocationFinder>();
}


void USf_PeakLocationFinder::RegisterActor(AActor* Actor)
{
	if (Actor &&
		!RegisteredActors.ContainsByPredicate([Actor](const FSf_ActorAssignment& Assignment) { return Assignment.AssignedActor == Actor; }))
	{
		RegisteredActors.Add({Actor, FVector::ZeroVector});
	}
}

void USf_PeakLocationFinder::UnregisterActor(AActor* Actor)
{
	RegisteredActors.RemoveAll([Actor](const FSf_ActorAssignment& Assignment) { return Assignment.AssignedActor == Actor; });
}

FVector USf_PeakLocationFinder::GetTargetLocationForActor(const AActor* Actor) const
{
	if (!IsValid(Actor))
		return FVector::ZeroVector;
	
	for (const FSf_ActorAssignment& Assignment : RegisteredActors)
	{
		if (Assignment.AssignedActor == Actor)
		{
			return Assignment.TargetPosition;
		}
	}
	
	return FVector::ZeroVector;
}


TArray<FSf_Circle> USf_PeakLocationFinder::GenerateCircleAroundPlayer(
    const int AmountOfRings,
    const int PointsPerRing,
    const float RingDistance,
    const int MinRingSize,
    const float MaxHeightDifference) const
{
    TArray<FSf_Circle> Circles;

    const APawn* PlayerPawn = USf_FunctionLibrary::GetSfPlayerpawn(GetWorld());
    if (!PlayerPawn)
        return Circles;

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    if (!NavSys)
        return Circles;

    for (int RingIndex = 0; RingIndex < AmountOfRings; ++RingIndex)
    {
        FSf_Circle NewCircle;

        float CircleRadius = MinRingSize + RingIndex * RingDistance;
        NewCircle.Radius = FVector(CircleRadius, 0, 0);

        for (int PointIndex = 0; PointIndex < PointsPerRing; ++PointIndex)
        {
            float AngleRadians = (2.0f * PI / PointsPerRing) * PointIndex;

            // Calculate the initial point on the circle
            FVector PointOnCircle = PlayerLocation + FVector(
                FMath::Cos(AngleRadians) * CircleRadius,
                FMath::Sin(AngleRadians) * CircleRadius,
                0.0f // No height offset initially
            );

            // Query navmesh for a valid location near this point
            FNavLocation NavLocation;
            if (NavSys->ProjectPointToNavigation(
                    PointOnCircle,
                    NavLocation,
                    FVector(50.0f, 50.0f, MaxHeightDifference))) // Navmesh query extent
            {
                // Ensure height difference from player is within the allowed range
                if (FMath::Abs(NavLocation.Location.Z - PlayerLocation.Z) <= MaxHeightDifference)
                {
                    PointOnCircle.Z = NavLocation.Location.Z; // Adjust to valid navmesh height
                    NewCircle.PointsOnCircle.Add(PointOnCircle);
                }
            }
        }

        Circles.Add(NewCircle);
    }

    return Circles;
}

void USf_PeakLocationFinder::DrawDebugCircles(const TArray<FSf_Circle>& Circles, UWorld* World, const FColor& CircleColor, const FColor& SphereColor, float SphereRadius, float Duration)
{
	if (!World) return;

	for (const FSf_Circle& Circle : Circles)
	{
		// Draw the circle outline
		DrawDebugCircle(
			World,
			Circle.PointsOnCircle.IsEmpty() ? FVector::ZeroVector : Circle.PointsOnCircle[0] - FVector(Circle.Radius.X, 0, 0), // Center of the circle
			Circle.Radius.X, // Radius
			50, // Number of segments
			CircleColor,
			false, // Persistent lines
			Duration, // Lifetime
			0, // Depth priority
			5.0f, // Thickness
			FVector(1, 0, 0), // X axis
			FVector(0, 1, 0), // Y axis
			false // Not filled
		);

		// Draw each point on the circle
		for (const FVector& Point : Circle.PointsOnCircle)
		{
			DrawDebugSphere(
				World,
				Point,
				SphereRadius,
				6,
				SphereColor, // Point color
				false, // Persistent points
				Duration // Lifetime
			);
		}
	}
}

void USf_PeakLocationFinder::UpdateActorAssignments()
{
    if (GeneratedCircles.Num() == 0 || RegisteredActors.Num() == 0)
        return;

    // Clear previous assignments
    for (FSf_ActorAssignment& Assignment : RegisteredActors)
        Assignment.TargetPosition = FVector::Zero();
	//
	
    // Get Player Location
    const APawn* PlayerPawn = USf_FunctionLibrary::GetSfPlayerpawn(GetWorld());
    if (!PlayerPawn)
        return;
    FVector PlayerLocation = PlayerPawn->GetActorLocation();
	//

	// Sort actors by proximity to the player
	RegisteredActors.Sort([PlayerLocation](const FSf_ActorAssignment& A, const FSf_ActorAssignment& B)
	{
		if (!A.AssignedActor || !B.AssignedActor)
			return false;
		return FVector::DistSquared(A.AssignedActor->GetActorLocation(), PlayerLocation) <
			   FVector::DistSquared(B.AssignedActor->GetActorLocation(), PlayerLocation);
	});
	//

	
	// Sort circles by proximity to the player
	TArray<FSf_Circle> SortedCircles = GeneratedCircles;
	SortedCircles.Sort([PlayerLocation](const FSf_Circle& A, const FSf_Circle& B)
	{
		if (A.PointsOnCircle.Num() == 0 || B.PointsOnCircle.Num() == 0)
			return false;
		FVector CenterA = A.PointsOnCircle[0];
		FVector CenterB = B.PointsOnCircle[0];
		return FVector::DistSquared(CenterA, PlayerLocation) < FVector::DistSquared(CenterB, PlayerLocation);
	});

	// Collect all positions, keeping circles in order
	TArray<FVector> AllPositions;
	for (const FSf_Circle& Circle : SortedCircles)
	{
		AllPositions.Append(Circle.PointsOnCircle);
	}
	
	// Allowable approach angle (degrees)
	float MaxDotProduct = FMath::Cos(FMath::DegreesToRadians(MaxApproachAngle));
	
	// Assign positions to actors
    TSet<FVector> AssignedPositions;
    for (FSf_ActorAssignment& Assignment : RegisteredActors)
    {
	    const AActor* Actor = Assignment.AssignedActor;
        if (!Actor)
            continue;

        FVector ActorLocation = Actor->GetActorLocation();
    	
    	// Sort positions by proximity to the actor
    	/*TArray<FVector> SortedPositions = AllPositions.FilterByPredicate(
		[&AssignedPositions](const FVector& Position) { return !AssignedPositions.Contains(Position); }
		);

    	SortedPositions.Sort([ActorLocation](const FVector& A, const FVector& B) {
			return FVector::DistSquared(A, ActorLocation) < FVector::DistSquared(B, ActorLocation);
		});*/
    	//

        for (const FVector& Position : AllPositions)
        {
            if (AssignedPositions.Contains(Position))
                continue;

            // Check for line-of-sight blocking
            bool bIsBlocked = false;
            constexpr float AngleThreshold = 60.0f; // The threshold in degrees
        	const float CosineThreshold = FMath::Cos(FMath::DegreesToRadians(AngleThreshold));
            for (const FSf_ActorAssignment& OtherAssignment : RegisteredActors)
            {
                if (OtherAssignment.AssignedActor == Actor || OtherAssignment.TargetPosition.IsZero())
                    continue;

            	const float ActorRadius = (Position - PlayerLocation).Length();  
            	const float OtherActorRadius = (OtherAssignment.TargetPosition - PlayerLocation).Length();
            	constexpr  float RadiusTolerance = 20.0f;

            	//Only check line of sight if dots are on same circle
            	if (FMath::Abs(ActorRadius - OtherActorRadius) < RadiusTolerance)
            	{
            		const FVector ToPlayer = (Position - PlayerLocation).GetSafeNormal();
            		const FVector ToOtherActor = (OtherAssignment.TargetPosition - PlayerLocation).GetSafeNormal();
            		const float DotProduct = FVector::DotProduct(ToPlayer, ToOtherActor);
		            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Message");
            		if (DotProduct > CosineThreshold) 
            		{
            			bIsBlocked = true;
            			break;
            		}
            	}
            	
            }
        	
        	FVector ToActor = (ActorLocation - PlayerLocation).GetSafeNormal();
        	FVector ToPosition = (Position - PlayerLocation).GetSafeNormal();
        	float DotProduct = FVector::DotProduct(ToActor, ToPosition);

        	// If DotProduct is less than MaxDotProduct, the position is outside the allowed angle range
        	if (DotProduct < MaxDotProduct)
        		continue;
        	
        	if (bIsBlocked)
        		continue;


            Assignment.TargetPosition = Position;
            AssignedPositions.Add(Position);
            break;
            
        }
    }
}
