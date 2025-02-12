#include "CF_Locomotion.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointSubsystem.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UCF_Locomotion::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_Locomotion_Config,LocomotionConfig)
}

bool UCF_Locomotion::MoveToLocation(const F_SF_MoveRequest MoveRequest)
{
	
	AAIController* OwningController = GetOwningAIController();
	FVector ProjectedDestinationLocation;
	EPathFollowingRequestResult::Type Result;

	if(MoveRequest.bMoveToActor)
	{
		if(!IsValid(MoveRequest.TargetActor))
		{
			USF_DebugFunctionLibrary::Sf_ThrowError(this, "Target Actor is invalid");
			//UE_LOG(EF_Locomotion, Error, TEXT("Target Actor is invalid"))
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
			const FString ErrorString = FString::Printf(TEXT("Failed to project location with radius: %f"),MoveRequest.ProjectionRadius);
			USF_DebugFunctionLibrary::Sf_ThrowError(this, ErrorString);
			//UE_LOG(EF_Locomotion, Warning, TEXT("Faild to project location"))
			return false;
		}
		ProjectedDestinationLocation = ProjectedDestination.Location;
		
		Result =  OwningController->MoveToLocation(
		ProjectedDestinationLocation,
		MoveRequest.AcceptanceRadius,
		true,
		true,
		true,
		true,
		MoveRequest.FilterClass,
		true
		);
	}
	

	if (Result ==EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		OnMoveStarted_CPP.Broadcast();
		OnMoveStarted_BP.Broadcast();
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
		USF_DebugFunctionLibrary::Sf_ThrowError(this, "Failed path request");
		USF_DebugFunctionLibrary::Sf_DrawDebugSphere(
			this,
			Sf_GameplayTags::Debug::TP::CharacterFeatures::Locomotion,
			MoveRequest.Destination,
			100,
			12,
		FColor::Red,
		40);
		return false;
	}
	
	if (!MoveRequest.bMoveToActor)
	{
		//NavTargetSys->RegisterNavTarget(ProjectedDestinationLocation);
		LastDestination = ProjectedDestinationLocation;
	}
	else if (MoveRequest.TargetActor)
	{
		LastDestination = MoveRequest.TargetActor->GetActorLocation();
	}


	OnMoveStarted_CPP.Broadcast();
	OnMoveStarted_BP.Broadcast();
	ProcessLocomotionType(MoveRequest.LocomotionType);
	OwningController->ReceiveMoveCompleted.AddDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	return true;
}

void UCF_Locomotion::StopMovement()
{
	AAIController* OwningController = GetOwningAIController();
	OwningController->StopMovement();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	ClearMoveFailAndFinishDelegates();
}

void UCF_Locomotion::FinishMovement()
{
	AAIController* OwningController = GetOwningAIController();
	OwningController->StopMovement();
	OwningController->ReceiveMoveCompleted.RemoveDynamic(this, &UCF_Locomotion::OnMoveCompleted);
	ASf_TetherPointGen* TetherPointGen =  USf_TetherPointSubsystem::Get(GetWorld())->GetTetherPointGen();
	OnMoveFinished_CPP.Broadcast();
	OnMoveFinished_BP.Broadcast();
	ClearMoveFailAndFinishDelegates();
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
	ClearMoveFailAndFinishDelegates();
}

void UCF_Locomotion::ClearMoveFailAndFinishDelegates()
{
	OnMoveFinished_CPP.Clear();
	OnMoveFinished_BP.Clear();
	OnMoveFailed_BP.Clear();
	OnMoveFailed_CPP.Clear();
}

