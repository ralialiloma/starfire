#include "CF_Cover.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/CollisionData.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugSubsystem.h"

bool UCF_Cover::EnterCover()
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

bool UCF_Cover::ExitCover()
{
	GetOwningCharacter()->UnCrouch();
	bIsInCoverState = false;
	return true;
}

bool UCF_Cover::CanBeHitByPlayer() const
{
	return CanBeHitByPlayer(MinCoverHeight);
}

bool UCF_Cover::IsInCoverState() const
{
	return bIsInCoverState;
}

bool UCF_Cover::CanBeHitByPlayer(float HeightOffset) const
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

	if (!HitActor->IsA(ASf_FP_Character::StaticClass()))
		return true;

	return false;
}
