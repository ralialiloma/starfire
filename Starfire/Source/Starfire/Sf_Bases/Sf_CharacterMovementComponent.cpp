// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Starfire/Shared/ActionLogger/ActionLogger.h"


void USf_CharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (MovementMode != NewMovementMode || CustomMovementMode != NewCustomMode)
	{
		const EMovementMode PreviousMovementMode = MovementMode;

		Super::SetMovementMode(NewMovementMode, NewCustomMode);
		OnMovementModeChanged.Broadcast(PreviousMovementMode, MovementMode);
	}
	Super::SetMovementMode(NewMovementMode, NewCustomMode);
}

bool USf_CharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return MovementMode == InMovementMode;
}

float USf_CharacterMovementComponent::CapRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float USf_CharacterMovementComponent::CapHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void USf_CharacterMovementComponent::ReportAction(const FGameplayTag ActionTag)
{
	const FActionLog ActionLog =  FActionLog(ActionTag, GetOwner()->GetRootComponent()->GetComponentLocation());
	UActionLoggerSubSystem::Get(GetWorld())->ReportAction(ActionLog);
}

