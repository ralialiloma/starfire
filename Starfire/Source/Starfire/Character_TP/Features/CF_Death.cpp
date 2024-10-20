#include "CF_Death.h"
#include "CF_Combat.h"
#include "CF_Locomotion.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"


void UCF_Death::Kill()
{
	//Handle Combat
	UCF_Combat* Combat =  GetOwningCharacter()->GetFeatureByClass<UCF_Combat>();
	if (IsValid(Combat))
	{
		Combat->StopFire();
		Combat->StopReload();
	}
	else
	{
		UE_LOG(EF_Death, Error, TEXT("Could not find %s"),*UCF_Combat::StaticClass()->GetName())
	}

	//Movement
	UCF_Locomotion* Locomotion =  GetOwningCharacter()->GetFeatureByClass<UCF_Locomotion>();
	if (IsValid(Locomotion))
	{
		Combat->StopFire();
		Combat->StopReload();
	}
	else
	{
		UE_LOG(EF_Death, Error, TEXT("Could not find %s"),*UCF_Locomotion::StaticClass()->GetName())
	}

	Locomotion->StopMovement();
	GetOwningSfMovement()->StopMovementImmediately();
	GetOwningSfMovement()->SetMovementMode(MOVE_None);

	//Focus
	GetOwningAIController()->ClearFocus(EAIFocusPriority::Gameplay);

	//Drop Weapon
	if (GetOwningSfEquipment()->IsEquipped())
		GetOwningSfEquipment()->RemoveWeaponActiveWeapon();
	
	//Pawn Message
	GetOwningCharacter()->PawnDeath();

	//Collision
	GetOwningCharacter()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//Destroy Character
	GetOwningCharacter()->Destroy();
}
