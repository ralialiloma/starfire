﻿#include "CF_Cover.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/CollisionData.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointGen.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UCF_Cover::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	bIsInHighCoverState = false;
	VALIDATE_CONFIG(UCF_Cover_Config,CoverConfig)
}

bool UCF_Cover::EnterCover()
{
	ASf_TetherPointGen* TetherPointGen = ASf_TetherPointGen::Get(this);
	if (!IsValid(TetherPointGen))
		return false;
	FGameplayTagContainer ActivePatrolPointTags =  TetherPointGen->GetClosestTetherPointTags(GetOwnerLocation());
	if (ActivePatrolPointTags.HasTag(Sf_GameplayTags::Gameplay::PatrolAreaMarkerTypes::Cover::Low))
		GetOwningCharacter()->Crouch();
	return true;
	
	/*GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Enter Cover");
	const bool bInHighCover = !CanBeHitByPlayer(CoverConfig->MinCoverHeight);
	if (bInHighCover)
	{
		if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Cover,EDebugVisualType::Print))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Character is in high cover");
		bIsInHighCoverState = true;
		bIsInLowCoverState = false;
		return true;
	}

	const bool bInLowCover = !CanBeHitByPlayer(CoverConfig->MaxCrouchCoverHeight+0.001f);
	if (bInLowCover)
	{
		if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Cover,EDebugVisualType::Print))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Character is in low cover");
		GetOwningCharacter()->GetSf_TP_CharacterMovement()->bWantsToCrouch = true;
		bIsInLowCoverState = true;
		bIsInHighCoverState = false;
		return true;
	}

	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Cover,EDebugVisualType::Print))
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Failed to enter cover");
	ExitCover();
	bIsInHighCoverState = false;
	bIsInLowCoverState = false;
	return false;*/
}

bool UCF_Cover::VerifyCover(const FVector LocationToVerify)
{
	const float CapHh = GetOwningCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	//Verify High Cover
	const FVector AdditionalHighCoverZHeight = FVector(0,0,CoverConfig->MaxCrouchCoverHeight-CapHh);
	const FVector HighCoverStart = LocationToVerify+AdditionalHighCoverZHeight;
	if (!CanBeHitByPlayer(HighCoverStart))
		return true;

	//Verify Low Cover
	const FVector AdditionalLowCoverZHeight = FVector(0,0,CoverConfig->MaxCrouchCoverHeight-CapHh);
	const FVector LowCoverStart = LocationToVerify+AdditionalLowCoverZHeight;
	if (!CanBeHitByPlayer(LowCoverStart))
		return true;

	return false;
}

bool UCF_Cover::ExitCover()
{
	if (GetOwningCharacter()->bIsCrouched)
		GetOwningCharacter()->GetSf_TP_CharacterMovement()->bWantsToCrouch = false;
	bIsInHighCoverState = false;
	bIsInLowCoverState = false;
	return true;
}

bool UCF_Cover::IsInHighCoverState() const
{
	return bIsInHighCoverState;
}

bool UCF_Cover::IsInCoverState() const
{
	return  bIsInLowCoverState|bIsInHighCoverState;
}

bool UCF_Cover::IsInLowCoverState() const
{
	return bIsInLowCoverState;
}
bool UCF_Cover::CanBeHitByPlayer(FVector LocationToVerify) const
{
	FVector Start = LocationToVerify;
	FVector End = USf_FunctionLibrary::GetPlayerLocation(this);
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{};
	bool ShowDebug = USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::CharacterFeatures::Cover,EDebugVisualType::Visual);

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


bool UCF_Cover::CanBeHitByPlayer(const float HeightOffset) const
{
	const float CapHh = GetOwningCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector AdditionalZHeight = FVector(0,0,HeightOffset-CapHh);
	const FVector Start = GetOwnerLocation()+AdditionalZHeight;
	return CanBeHitByPlayer(Start);
}
