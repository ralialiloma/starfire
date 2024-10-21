#include "CF_Cover.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/CollisionData.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugSubsystem.h"

void UCF_Cover::Initialize(ASf_TP_Character* Holder)
{
	Super::Initialize(Holder);
	bIsInCoverState = false;
}

bool UCF_Cover::EnterCover()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Enter Cover");

	
	if (bIsInCoverState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Already in Cover State");
		return true;
	}

	const bool bInHighCover = !CanBeHitByPlayer(/*MinCoverHeight*/120);
	const bool bInLowCover = !CanBeHitByPlayer( 0/*MaxCrouchCoverHeight+0.001f*/);
	
	if (!bInHighCover && bInLowCover)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Character is in low cover");
		if (!GetOwningCharacter()->bIsCrouched)
			GetOwningCharacter()->Crouch();
		bIsInCoverState = true;
		return true;
	}

	if (bInHighCover)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Character is in high cover");
		bIsInCoverState = true;
		return true;
	}
	
	
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Failed to enter cover");
	bIsInCoverState = false;
	return false;
}

bool UCF_Cover::ExitCover()
{
	if (GetOwningCharacter()->bIsCrouched)
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
	float CapHh = GetOwningCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector AdditionalZHeight = FVector(0,0,HeightOffset-CapHh);
	FVector Start = GetOwnerLocation()+AdditionalZHeight;
	FVector End = USf_FunctionLibrary::GetPlayerLocation(this);
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
		ShowDebug?5:0
		);
	
	if (!HitResult.bBlockingHit)
		return false;

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor) || !HitActor->IsA(ASf_FP_Character::StaticClass()))
		return false;

	return true;
}
