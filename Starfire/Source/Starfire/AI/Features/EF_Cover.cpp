#include "EF_Cover.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Starfire/Utility/CollisionData.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

bool UEF_Cover::EnterCover()
{
	if (bIsInCoverState)
	{
		return true;
	}
	
	if (CanBeHitByPlayer(MinCoverHeight))
	{
		bIsInCoverState = false;
		return false;
	}
		
	if (CanBeHitByPlayer(MaxCrouchCoverHeight+0.001f))
	{
		GetOwningCharacter()->Crouch();
		bIsInCoverState = true;
		return true;
	}

	bIsInCoverState = true;
	return true;
}

bool UEF_Cover::ExitCover()
{
	GetOwningCharacter()->UnCrouch();
	bIsInCoverState = false;
	return true;
}

bool UEF_Cover::CanBeHitByPlayer() const
{
	return CanBeHitByPlayer(MinCoverHeight);
}

bool UEF_Cover::IsInCoverState() const
{
	return bIsInCoverState;
}

bool UEF_Cover::CanBeHitByPlayer(float HeightOffset) const
{
	FVector Start = GetOwnerLocation()+HeightOffset;
	FVector End = USf_FunctionLibrary::GetPlayerLocation(this)+HeightOffset;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{};
	bool ShowDebug = UDebugSubsystem::GetAIDebug(EDebugType::Visual);

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(EEC_BULLETTRACES);

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		TraceType,
		false,
		GetIgnoreActors(),
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FColor::Red,
		FColor::Green,
		ShowDebug?1:0
		);
	
	if (!HitResult.bBlockingHit)
		return true;

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
		return true;

	if (!HitActor->IsA(ASf_Character::StaticClass()))
		return true;

	return false;
}
