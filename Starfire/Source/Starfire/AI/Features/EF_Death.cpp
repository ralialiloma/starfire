#include "EF_Death.h"
#include "EF_Combat.h"
#include "EF_Locomotion.h"
#include "Starfire/AI/Sf_NPCharacter.h"


void UEF_Death::Kill()
{
	//Handle Combat
	UEF_Combat* Combat =  GetOwningCharacter()->GetFeatureByClass<UEF_Combat>();
	if (IsValid(Combat))
	{
		Combat->StopFire();
		Combat->StopReload();
	}
	else
	{
		UE_LOG(EF_Death, Error, TEXT("Could not find %s"),*UEF_Combat::StaticClass()->GetName())
	}

	//Movement
	UEF_Locomotion* Locomotion =  GetOwningCharacter()->GetFeatureByClass<UEF_Locomotion>();
	if (IsValid(Locomotion))
	{
		Combat->StopFire();
		Combat->StopReload();
	}
	else
	{
		UE_LOG(EF_Death, Error, TEXT("Could not find %s"),*UEF_Locomotion::StaticClass()->GetName())
	}

	Locomotion->StopMovement();
	GetOwningSfMovement()->StopMovementImmediately();
	GetOwningSfMovement()->SetMovementMode(MOVE_None);

	//Focus
	GetOwningAIController()->ClearFocus(EAIFocusPriority::Gameplay);
	
	//Pawn Message
	GetOwningCharacter()->PawnDeath();

	//Collision
	GetOwningCharacter()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//Destroy Character
	GetOwningCharacter()->Destroy();
}
