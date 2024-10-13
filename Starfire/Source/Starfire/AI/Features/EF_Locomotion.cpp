#include "EF_Locomotion.h"

#include "Navigation/PathFollowingComponent.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/AI/Sf_TP_CharacterMovementComponent.h"
#include "Starfire/AI/WeaponAnimMontageController_TP.h"
#include "Starfire/AI/EQS/NavigationTargetSubsystem.h"


bool UEF_Locomotion::MoveToLocation(FMoveRequest MoveRequest)
{
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();

	NavTargetSys->UnregisterReservedCover(LastDestination);
	const bool bLocationReserved =  NavTargetSys->LocationInReservedCover(MoveRequest.Destination);
	if (bLocationReserved)
		return false;
	NavTargetSys->RegisterReservedCover(MoveRequest.Destination);

	EPathFollowingRequestResult::Type Result =  GetOwningAIController()->MoveToLocation(
		MoveRequest.Destination,
		MoveRequest.AcceptanceRadius,
		true,
		true,
		true,
		false,
		MoveRequest.FilterClass,
		true
		);

	if (Result ==EPathFollowingRequestResult::Type::Failed ||Result ==EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		StopMoving();
		return false;
	}

	ProcessLocomotionType(MoveRequest.LocomotionType);

	return true;
}

void UEF_Locomotion::StopMoving()
{
	GetOwningAIController()->StopMovement();
	UNavigationTargetSubsystem* NavTargetSys = GetWorld()->GetGameInstance()->GetSubsystem<UNavigationTargetSubsystem>();
	NavTargetSys->UnregisterReservedCover(LastDestination);
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
