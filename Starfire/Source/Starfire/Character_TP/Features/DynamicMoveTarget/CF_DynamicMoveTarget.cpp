
#include "CF_DynamicMoveTarget.h"

#include "CF_DynamicMoveTarget_Config.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


void UCF_DynamicMoveTarget::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_DynamicMoveTarget_Config,DynamicMoveTargetConfig);
}

void UCF_DynamicMoveTarget::OnBeginPlay()
{
	Super::OnBeginPlay();

	UWorld* World = GetWorld();

	if (!IsValid(World))
		return;

	//USf_CloseToPlayerLoc::GetCurrent(GetWorld())->RegisterActor(GetOwningCharacter());

//	USf_PeakLocationFinder::GetCurrent(GetWorld())->RegisterActor(GetOwningCharacter());
	

	ACharacter* Character =  GetOwningCharacter();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character; 
	SpawnParams.Instigator = Character->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector SpawnLocation = Character->GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	MoveTarget = World->SpawnActor<ASf_TP_DynamicMoveTarget>(ASf_TP_DynamicMoveTarget::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	
	TimeSinceLastUpdate   = 0.0f;
}

void UCF_DynamicMoveTarget::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	if(!bRegisteredForTetherPointGen)
	{
		if (USf_TetherPointSubsystem::Get(GetWorld())->GetTetherPointGen())
		{
			USf_TetherPointSubsystem::Get(GetWorld())->GetTetherPointGen()->RegisterActor(GetOwningCharacter());
			bRegisteredForTetherPointGen = true;
		}
	}

//	MoveTarget->SetActorLocation(USf_PeakLocationFinder::GetCurrent(GetWorld())->GetTargetLocationForActor(GetOwningCharacter()));

	/*if (!IsValid(DynamicMoveTargetConfig))
		return;

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < 1.0f/DynamicMoveTargetConfig->FrameRate)
		return;

	TimeSinceLastUpdate = 0.0f;

	const ACharacter* Character =  GetOwningCharacter();
	if (!IsValid(Character))
		return;*/
	

	/*const FVector MovingActorLocation = Character->GetActorLocation();
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const float Distance = FVector::Distance(MovingActorLocation, PlayerLocation);

	float MinDistance = DynamicMoveTargetConfig->MinDistance;
	const float MaxDistance = DynamicMoveTargetConfig->MaxDistance;

	bool bTooClose = Distance < MinDistance;
	bool bTooFar =  Distance > MaxDistance;
	constexpr float MinMoveDistance = 300.f;
	//if (!USf_CloseToPlayerLoc::GetCurrent(GetWorld())->ValidateCloseToPlayerLoc(MoveTarget->GetActorLocation(),MinMoveDistance))
	{
		const FVector Location =  USf_CloseToPlayerLoc::GetCurrent(GetWorld())->GetCloseToPlayerLoc(Character);

		//if (FVector::Distance(MoveTarget->GetActorLocation(),Location)>MinMoveDistance)
			MoveTarget->SetActorLocation(Location);
	}*/


	/*const FVector MovingActorLocation = Character->GetActorLocation();
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const float Distance = FVector::Distance(MovingActorLocation, PlayerLocation);

	float MinDistance = DynamicMoveTargetConfig->MinDistance;
	const float MaxDistance = DynamicMoveTargetConfig->MaxDistance;

	bool bTooClose = Distance < MinDistance;
	bool bTooFar =  Distance > MaxDistance;
	if (/*bTooClose ||bTooFar/true)
	{
		/*TArray<FVector> Locations =  USf_CloseToPlayerLoc::GetCurrent(GetWorld())->GetCurrentCloseToPlayerLocations();
		if (Locations.Num()<=0)
		{
			MoveTarget->SetActorLocation(MovingActorLocation);
			return;
		}*/
		
	/*	FVector Direction = (MovingActorLocation-PlayerLocation);
		Direction.Normalize();

		const float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
		const float TempMinDistance = CapsuleRadius+MinDistance;
		const float TempMaxDistance = CapsuleRadius+MaxDistance;
		const float DesiredDistance = (TempMinDistance + TempMaxDistance) / 2.0f;
		
		FVector Location = MovingActorLocation + Direction * (DesiredDistance - Distance);
		//Raycast to Floor
		//FHitResult HitResult;

		FCollisionQueryParams CollisionParams = FCollisionQueryParams();
		
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
		    Location,
		    Location + FVector::DownVector * DynamicMoveTargetConfig->MaxVerticalTraceLength,
		    ECollisionChannel::ECC_WorldStatic,
		    CollisionParams
		);*/

		/*UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSys)
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Navigation system not found!");
			return;
		}

		if (!DynamicMoveTargetConfig || DynamicMoveTargetConfig->ProjectionExtent.IsNearlyZero())
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid DynamicMoveTargetConfig or ProjectionExtent!");
			return;
		}

		ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
		if (!NavData)
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid NavData!");
			return;
		}*/

		//Remove All Reserved Locations
		/*TArray<FVector>  TempLocations = Locations;
		for (FVector Location: TempLocations)
		{
			if(UNavigationTargetSubsystem::Get(GetWorld())->HasCloseNavTarget(Location,300.f,GetOwningCharacter()))
				Locations.Remove(Location);
		}

		if (Locations.Num() <=0)
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"all points already targeted");
			return;
		}
		
		if (bTooFar)
		{
			Locations.Sort([&MovingActorLocation, &PlayerLocation](const FVector& A, const FVector& B) {
			constexpr float WeightAIToPoint = 0.6f;
			constexpr float WeightAIToPlayer = 0.4f;
			constexpr float WeightPlayerToAI = 0.2f;
				
					
			float ScoreA = WeightAIToPoint * FVector::DistSquared(MovingActorLocation, A) +
						   WeightAIToPlayer * FVector::DistSquared(PlayerLocation, A) +
						   WeightPlayerToAI * FVector::DistSquared(PlayerLocation, MovingActorLocation);

			float ScoreB = WeightAIToPoint * FVector::DistSquared(MovingActorLocation, B)+
							WeightAIToPlayer * FVector::DistSquared(PlayerLocation, B);
					
				return ScoreA < ScoreB;
			});
		}
		else if (bTooClose)
		{
			Locations.Sort([&MovingActorLocation, &PlayerLocation](const FVector& A, const FVector& B) {
			constexpr float WeightToAI = 0.6f;
			float WeightToPlayer = 0.4f; 
					
			float ScoreA = WeightToAI * FVector::DistSquared(MovingActorLocation, A) -
						   WeightToPlayer * FVector::DistSquared(PlayerLocation, A);
			float ScoreB = WeightToAI * FVector::DistSquared(MovingActorLocation, B)-
							WeightToPlayer * FVector::DistSquared(PlayerLocation, B);
					
				return ScoreA < ScoreB;
			});
		}*/
		
		
		//Make sure point is rechable for Character
		/*bool bPointFound = false;
		FVector TraceHit;
		for (FVector Point: Locations)
		{
			FPathFindingQuery Query;
			Query.StartLocation = MovingActorLocation;
			Query.EndLocation = Point;
			Query.NavData = NavData;

			if (NavSys->TestPathSync(Query))
			{
				TraceHit = Point;
				bPointFound = true;
				break;
			};
		}

		if (!bPointFound)
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"None of the points are reachable");
			return;
		}
		
		FNavLocation ProjectedLocation;
		if (!NavSys->ProjectPointToNavigation(TraceHit, ProjectedLocation, DynamicMoveTargetConfig->ProjectionExtent))
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Failed to project to navigation!");
			UKismetSystemLibrary::DrawDebugSphere(this,TraceHit,150.f,6,FColor::Red,10,2);
		}
		
		MoveTarget->SetActorLocation(ProjectedLocation);
		
	}
	else
	{
		//MoveTarget->SetActorLocation(MovingActorLocation);
	}*/
}

ASf_TP_DynamicMoveTarget* UCF_DynamicMoveTarget::GetMoveTarget() const
{
	return MoveTarget;
}


