
#include "CF_DynamicMoveTarget.h"

#include "CF_DynamicMoveTarget_Config.h"
#include "DebugFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


void UCF_DynamicMoveTarget::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_DynamicMoveTarget_Config,DynamicMoveTargetConfig);
}

void UCF_DynamicMoveTarget::OnBeginPlay_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	//USf_CloseToPlayerLoc::GetCurrent(GetWorld())->RegisterArea(GetOwningCharacter());

//	USf_PeakLocationFinder::GetCurrent(GetWorld())->RegisterArea(GetOwningCharacter());

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

void UCF_DynamicMoveTarget::OnTick_Implementation(float DeltaTime)
{
	if (!IsValid(MoveTarget))
		return;

	if (!TrySetTarget(DeltaTime))
	{
		MoveTarget->SetActorLocation(GetOwnerLocation());
	}


}

ASf_TP_DynamicMoveTarget* UCF_DynamicMoveTarget::GetMoveTarget() const
{
	return MoveTarget;
}



bool UCF_DynamicMoveTarget::TrySetTarget(float DeltaTime)
{
	if (!IsValid(DynamicMoveTargetConfig))
		return false;

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < 1.0f/DynamicMoveTargetConfig->FrameRate)
		return true;

	TimeSinceLastUpdate = 0.0f;

	const ACharacter* Character =  GetOwningCharacter();
	if (!IsValid(Character))
		return false;

	const FVector MovingActorLocation = Character->GetActorLocation();
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const float Distance = FVector::Distance(MovingActorLocation, PlayerLocation);
	float MinDistance = DynamicMoveTargetConfig->MinDistance;
	const float MaxDistance = DynamicMoveTargetConfig->MaxDistance;
	bool bTooClose = Distance < MinDistance;
	bool bTooFar =  Distance > MaxDistance;

	if (!bTooClose && !bTooFar)
	{
		return false;
	}
	
	FVector Direction = (MovingActorLocation-PlayerLocation);
	Direction.Normalize();

	const float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float TempMinDistance = CapsuleRadius+MinDistance;
	const float TempMaxDistance = CapsuleRadius+MaxDistance;
	const float DesiredDistance = TempMinDistance;
	FVector Location = MovingActorLocation + Direction * (DesiredDistance - Distance);

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
	    Location,
	    Location + FVector::DownVector * DynamicMoveTargetConfig->MaxVerticalTraceLength,
	    ECollisionChannel::ECC_WorldStatic,
	    CollisionParams
	);

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		UDebugFunctionLibrary::DebugError(this,"Navigation system not found!");
		return false;
	}

	if (!DynamicMoveTargetConfig || DynamicMoveTargetConfig->ProjectionExtent.IsNearlyZero())
	{
		UDebugFunctionLibrary::DebugError(this,"Invalid DynamicMoveTargetConfig or ProjectionExtent!");
		return false;
	}

	ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData)
	{
		UDebugFunctionLibrary::DebugError(this,"Invalid NavData!");
		return false;
	}
	
	
	//Make sure point is rechable for Character
	FVector TraceHit = HitResult.Location;
	FPathFindingQuery Query;
	Query.StartLocation = MovingActorLocation;
	Query.EndLocation = TraceHit;
	Query.NavData = NavData;
	Query.bAllowPartialPaths = false;
	Query.QueryFilter =  UNavigationQueryFilter::GetQueryFilter(*NavData, GetOwningActor(), DynamicMoveTargetConfig->NavigationFilterClass);


	if (!NavSys->TestPathSync(Query))
	{
		return false;
	};
	
	FNavLocation ProjectedLocation;
	if (!NavSys->ProjectPointToNavigation(TraceHit, ProjectedLocation, DynamicMoveTargetConfig->ProjectionExtent))
	{
		UKismetSystemLibrary::DrawDebugSphere(this,TraceHit,150.f,6,FColor::Red,10,2);
	}
	
	MoveTarget->SetActorLocation(ProjectedLocation);
	return true;
	
}


