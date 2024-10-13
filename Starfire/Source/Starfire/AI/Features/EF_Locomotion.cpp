#include "EF_Locomotion.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/AI/Sf_TP_CharacterMovementComponent.h"
#include "Starfire/AI/WeaponAnimMontageController_TP.h"
#include "Starfire/AI/EQS/NavigationTargetSubsystem.h"


bool UEF_Locomotion::MoveToLocation(FMoveRequest MoveRequest)
{
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();

	FNavLocation ProjectedDestination;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	bool bProjected = NavSys->ProjectPointToNavigation(
		MoveRequest.Destination,           
		ProjectedDestination,
		FVector(1000, 1000, 1000));

	FVector ProjectedDestinationLocation = ProjectedDestination.Location;
	
	AAIController* OwningController = GetOwningAIController();

	NavTargetSys->UnregisterReservedCover(LastDestination);
	const bool bLocationReserved =  NavTargetSys->LocationInReservedCover(ProjectedDestinationLocation);
	if (bLocationReserved)
	{
		ClearAllDelegates();
		UE_LOG(LogTemp, Log, TEXT("Location already reserved"))
		return false;
	}

	EPathFollowingRequestResult::Type Result =  OwningController->MoveToLocation(
		ProjectedDestinationLocation,
		MoveRequest.AcceptanceRadius,
		true,
		true,
		true,
		false,
		MoveRequest.FilterClass,
		true
		);

	if (Result ==EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		OnMoveFinished_CPP.Broadcast();
		OnMoveFinished_BP.Broadcast();
		StopMovement();
		UE_LOG(LogTemp, Log, TEXT("Already at goal"))
		return false;
	}

	if (Result ==EPathFollowingRequestResult::Type::Failed)
	{
		OnMoveFailed_BP.Broadcast();
		OnMoveFailed_CPP.Broadcast();
		StopMovement();
		UE_LOG(LogTemp, Log, TEXT("Failed to reach location"))
		return false;
	}
	
	NavTargetSys->RegisterReservedCover(ProjectedDestinationLocation);
	LastDestination = ProjectedDestinationLocation;
	ProcessLocomotionType(MoveRequest.LocomotionType);
	OwningController->ReceiveMoveCompleted.AddDynamic(this, &UEF_Locomotion::OnMoveCompleted);
	return true;
}

void UEF_Locomotion::StopMovement()
{
	AAIController* OwningController = GetOwningAIController();
	
	OwningController->StopMovement();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UEF_Locomotion::OnMoveCompleted);
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	NavTargetSys->UnregisterReservedCover(LastDestination);
	ClearAllDelegates();
}

void UEF_Locomotion::ProcessLocomotionType(E_NP_LocomotionType LocomotionType)
{
	USf_TP_CharacterMovementComponent* MovementComponent =  GetOwningSfMovement();
	
	switch (LocomotionType)
	{
		case E_NP_LocomotionType::Crouch:
			MovementComponent->SprintReleased();
			MovementComponent->Crouch();
			break;
		case E_NP_LocomotionType::Walk:
			MovementComponent->UnCrouch();
			MovementComponent->SprintReleased();
			break;
		case E_NP_LocomotionType::Sprint:
			MovementComponent->UnCrouch();
			MovementComponent->SprintPressed();
				break;
		case E_NP_LocomotionType::KeepCurrent:
			break;
		default:
			break;
	}
}

void UEF_Locomotion::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	AAIController* OwningController = GetOwningAIController();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UEF_Locomotion::OnMoveCompleted);

	if (Result == EPathFollowingResult::Success)
	{
		OnMoveFinished_CPP.Broadcast();
		OnMoveFinished_BP.Broadcast();
		return;
	}
	OnMoveFailed_BP.Broadcast();
	OnMoveFailed_CPP.Broadcast();
	ClearAllDelegates();
}

void UEF_Locomotion::ClearAllDelegates()
{
	OnMoveFinished_CPP.Clear();
	OnMoveFinished_BP.Clear();
	OnMoveFailed_BP.Clear();
	OnMoveFailed_CPP.Clear();
}

