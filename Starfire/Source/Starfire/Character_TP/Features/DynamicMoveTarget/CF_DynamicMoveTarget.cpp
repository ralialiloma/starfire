
#include "CF_DynamicMoveTarget.h"

#include "CF_DynamicMoveTarget_Config.h"
#include "NavigationSystem.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/CollisionData.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


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

	if (!IsValid(DynamicMoveTargetConfig))
		return;

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < 1.0f/DynamicMoveTargetConfig->FrameRate)
		return;
	
	const ACharacter* Character =  GetOwningCharacter();
	if (!IsValid(Character))
		return;

	const FVector MovingActorLocation = Character->GetActorLocation();
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const float Distance = FVector::Distance(MovingActorLocation, PlayerLocation);

	float MinDistance = DynamicMoveTargetConfig->MinDistance;
	const float MaxDistance = DynamicMoveTargetConfig->MaxDistance;
	if (Distance < MinDistance || Distance > MaxDistance)
	{
		FVector Direction = (MovingActorLocation-PlayerLocation);
		Direction.Normalize();

		const float CapsuleRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
		const float TempMinDistance = CapsuleRadius+MinDistance;
		const float TempMaxDistance = CapsuleRadius+MaxDistance;
		const float DesiredDistance = (TempMinDistance + TempMaxDistance) / 2.0f;
		
		FVector Location = MovingActorLocation + Direction * (DesiredDistance - Distance);
		//Raycast to Floor
		FHitResult HitResult;

		FCollisionQueryParams CollisionParams = FCollisionQueryParams();
		
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
			UDebugFunctionLibrary::Sf_ThrowError(this,"Navigation system not found!");
			return;
		}

		if (!DynamicMoveTargetConfig || DynamicMoveTargetConfig->ProjectionExtent.IsNearlyZero())
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid DynamicMoveTargetConfig or ProjectionExtent!");
			return;
		}

		FVector TraceHit = HitResult.Location;
		FNavLocation ProjectedLocation;
		if (!NavSys->ProjectPointToNavigation(TraceHit, ProjectedLocation, DynamicMoveTargetConfig->ProjectionExtent))
		{
			UDebugFunctionLibrary::Sf_ThrowError(this,"Failed to project to navigation!");
		}
		
		MoveTarget->SetActorLocation(ProjectedLocation.Location);
	}
	else
	{
		MoveTarget->SetActorLocation(MovingActorLocation);
	}
	TimeSinceLastUpdate = 0.0f;
}

ASf_TP_DynamicMoveTarget* UCF_DynamicMoveTarget::GetMoveTarget() const
{
	return MoveTarget;
}


