
#include "CF_DynamicMoveTarget.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"


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

	// Spawn the MoveTarget actor
	MoveTarget = World->SpawnActor<ASf_TP_DynamicMoveTarget>(ASf_TP_DynamicMoveTarget::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	
	TimeSinceLastUpdate   = 0.0f;
}

void UCF_DynamicMoveTarget::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < 1.0f/FrameRate)
		return;
	
	const ACharacter* Character =  GetOwningCharacter();
	if (!IsValid(Character))
		return;

	const FVector MovingActorLocation = Character->GetActorLocation();
	const FVector PlayerLocation = USf_FunctionLibrary::GetPlayerLocation(this);
	const float Distance = FVector::Distance(MovingActorLocation, PlayerLocation);
	
	if (Distance < MinDistance || Distance > MaxDistance)
	{
		FVector Direction = (MovingActorLocation-PlayerLocation);
		Direction.Normalize();
		
		const float DesiredDistance = (MinDistance + MaxDistance) / 2.0f;
		
		FVector TargetLocation = MovingActorLocation + Direction * (DesiredDistance - Distance);
		TargetLocation = FVector(TargetLocation.X, TargetLocation.Y,PlayerLocation.Z);
		
		MoveTarget->SetActorLocation(TargetLocation);
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

