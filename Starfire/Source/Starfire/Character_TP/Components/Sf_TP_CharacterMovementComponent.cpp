// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_TP_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"


USf_TP_CharacterMovementComponent::FSavedMove_Sf::FSavedMove_Sf(): Saved_bWantsToSprint(0)
{
}

bool USf_TP_CharacterMovementComponent::FSavedMove_Sf::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Sf* NewSfMove = static_cast<FSavedMove_Sf*>(NewMove.Get());

	if (Saved_bWantsToSprint!= NewSfMove->Saved_bWantsToSprint)
		return false;

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USf_TP_CharacterMovementComponent::FSavedMove_Sf::Clear()
{
	FSavedMove_Character::Clear();
	Saved_bWantsToSprint = 0;
}

void USf_TP_CharacterMovementComponent::FSavedMove_Sf::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	const USf_TP_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USf_TP_CharacterMovementComponent>(C->GetCharacterMovement());
	
	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
}

void USf_TP_CharacterMovementComponent::FSavedMove_Sf::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	USf_TP_CharacterMovementComponent* CharacterMovementComponent =
		Cast<USf_TP_CharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovementComponent->Safe_bWantsToSprint =  Saved_bWantsToSprint;
}

void USf_TP_CharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SfCharacterOwner = Cast<ASf_TP_Character>(CharacterOwner);
	RotationRate = DefaultRotationRate;
}

void USf_TP_CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,const FVector& OldVelocity)
{
	/*if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkspeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}*/

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

bool USf_TP_CharacterMovementComponent::CanSprint()
{
	return IsMovementMode(MOVE_Walking) && !IsFalling();
}

bool USf_TP_CharacterMovementComponent::IsSprinting()
{
	return Safe_bWantsToSprint;
}

void USf_TP_CharacterMovementComponent::SprintPressed()
{
	if (!CanSprint())
		return;
	
	Safe_bWantsToSprint = true;
}

void USf_TP_CharacterMovementComponent::SprintReleased()
{
	if(!Safe_bWantsToSprint)
		return;
	
	Safe_bWantsToSprint = false;
}

void USf_TP_CharacterMovementComponent::SetStrafe(const bool bWantsToStrafe)
{
	Safe_bWantsToStrafe = bWantsToStrafe;
	bUseControllerDesiredRotation = Safe_bWantsToStrafe;
	bOrientRotationToMovement = !Safe_bWantsToStrafe;
	RotationRate = IsFalling()? DefaultRotationRate:FallRotationRate;
}

bool USf_TP_CharacterMovementComponent::IsStrafing()
{
	return Safe_bWantsToStrafe;
}
