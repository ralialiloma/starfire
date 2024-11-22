#include "CF_Locomotion.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"


void UCF_Locomotion::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_Locomotion_Config,LocomotionConfig)
}

bool UCF_Locomotion::MoveToLocation(const F_SF_MoveRequest MoveRequest)
{
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();

	
	AAIController* OwningController = GetOwningAIController();
	FVector ProjectedDestinationLocation;
	EPathFollowingRequestResult::Type Result;

	if(MoveRequest.bMoveToActor)
	{
		if(!IsValid(MoveRequest.TargetActor))
		{
			UE_LOG(EF_Locomotion, Error, TEXT("Target Actor is invalid"))
			return false;
		}
		
		Result =  OwningController->MoveToActor(
			MoveRequest.TargetActor,
			MoveRequest.AcceptanceRadius,
			true,
			true,
			true,
			MoveRequest.FilterClass,
			true
		);
	}
	else
	{
		FNavLocation ProjectedDestination;
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
		bool bProjected = NavSys->ProjectPointToNavigation(
			MoveRequest.Destination,           
			ProjectedDestination,
			FVector(MoveRequest.ProjectionRadius, MoveRequest.ProjectionRadius, MoveRequest.ProjectionRadius));

		if (!bProjected)
		{
			return false;
		}
		ProjectedDestinationLocation = ProjectedDestination.Location;

		NavTargetSys->UnregisterReservedCover(LastDestination);
		const bool bLocationReserved =  NavTargetSys->LocationInReservedCover(ProjectedDestinationLocation);
		if (bLocationReserved)
		{
			ClearAllDelegates();
			UE_LOG(EF_Locomotion, Log, TEXT("Location already reserved"))
			return false;
		}
		
		Result =  OwningController->MoveToLocation(
		ProjectedDestinationLocation,
		MoveRequest.AcceptanceRadius,
		true,
		true,
		true,
		false,
		MoveRequest.FilterClass,
		true
		);
	}
	

	if (Result ==EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		OnMoveFinished_CPP.Broadcast();
		OnMoveFinished_BP.Broadcast();
		StopMovement();
		return true;
	}

	if (Result ==EPathFollowingRequestResult::Type::Failed)
	{
		OnMoveFailed_BP.Broadcast();
		OnMoveFailed_CPP.Broadcast();
		StopMovement();
		UE_LOG(EF_Locomotion, Warning, TEXT("Failed path request"))
		return false;
	}
	
	if (!MoveRequest.bMoveToActor)
	{
		NavTargetSys->RegisterReservedCover(ProjectedDestinationLocation);
		LastDestination = ProjectedDestinationLocation;
	}
	else if (MoveRequest.TargetActor)
	{
		LastDestination = MoveRequest.TargetActor->GetActorLocation();
	}

	
	ProcessLocomotionType(MoveRequest.LocomotionType);
	OwningController->ReceiveMoveCompleted.AddDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	return true;
}

void UCF_Locomotion::StopMovement()
{
	AAIController* OwningController = GetOwningAIController();
	
	OwningController->StopMovement();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	NavTargetSys->UnregisterReservedCover(LastDestination);
	ClearAllDelegates();
}

void UCF_Locomotion::FinishMovement()
{
	AAIController* OwningController = GetOwningAIController();
	OwningController->StopMovement();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	NavTargetSys->UnregisterReservedCover(LastDestination);
	
	OnMoveFinished_CPP.Broadcast();
	OnMoveFinished_BP.Broadcast();
	ClearAllDelegates();
}

void UCF_Locomotion::ProcessLocomotionType(E_Sf_TP_LocomotionType LocomotionType)
{
	USf_TP_CharacterMovementComponent* MovementComponent =  GetOwningSfMovement();
	
	switch (LocomotionType)
	{
		case E_Sf_TP_LocomotionType::Crouch:
			MovementComponent->SprintReleased();
			MovementComponent->bWantsToCrouch = true;
			break;
		case E_Sf_TP_LocomotionType::Walk:
			MovementComponent->bWantsToCrouch = false;
			MovementComponent->SprintReleased();
			break;
		case E_Sf_TP_LocomotionType::Sprint:
			MovementComponent->bWantsToCrouch = false;;
			MovementComponent->SprintPressed();
				break;
		case E_Sf_TP_LocomotionType::KeepCurrent:
			break;
		default:
			break;
	}
}

void UCF_Locomotion::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	AAIController* OwningController = GetOwningAIController();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UCF_Locomotion::OnMoveCompleted);

	if (Result == EPathFollowingResult::Success)
	{
		OnMoveFinished_CPP.Broadcast();
		OnMoveFinished_BP.Broadcast();
		return;
	}

	UE_LOG(EF_Locomotion, Error, TEXT("Move Failed"))
	OnMoveFailed_BP.Broadcast();
	OnMoveFailed_CPP.Broadcast();
	ClearAllDelegates();
}

void UCF_Locomotion::ClearAllDelegates()
{
	OnMoveFinished_CPP.Clear();
	OnMoveFinished_BP.Clear();
	OnMoveFailed_BP.Clear();
	OnMoveFailed_CPP.Clear();
}

