#include "EF_Cover.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

bool UEF_Cover::EnterCover()
{
	if (bIsInCover)
		return true;

	if ()
}

bool UEF_Cover::IsCovered() const
{
	FVector Start = GetOwnerLocation()+MinCoverHeight;
	FVector End = USf_FunctionLibrary::GetPlayerLocation(this)+MinCoverHeight;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{};
	bool ShowDebug = UDebugSubsystem::GetAIDebug(EDebugType::Visual);

	UKismetSystemLibrary::LineTraceSingleByProfile(
		this,
		Start,
		End,
		"BulletShots",
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

	return true;
		
}
