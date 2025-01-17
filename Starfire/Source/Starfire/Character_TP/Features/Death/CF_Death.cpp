#include "CF_Death.h"

#include "Animation/SkeletalMeshActor.h"
#include "Starfire/Character_TP/PoseAnimInstance.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Features/Combat/CF_Combat.h"
#include "Starfire/Character_TP/Features/Locomotion/CF_Locomotion.h"
#include "Starfire/Shared/Resources/Resource.h"


void UCF_Death::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_Death_Config,Death_Config)
}

void UCF_Death::Kill()
{
	//Broadcast Events
	OnDeath_CPP.Broadcast();
	OnDeath_BP.Broadcast();
	
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
	{
		EquippedWeapon = GetOwningSfEquipment()->GetActiveWeapon();
		float MontageTime;
		GetOwningSfEquipment()->RemoveWeaponActiveWeapon(MontageTime);
		EquippedWeapon->Destroy();
	}

	//Spawn Loot
	if (const TSubclassOf<AResource> DroppedResourceClass = Death_Config->DroppedResourceClass)
		GetOwningSfEquipment()->GetWorld()->SpawnActor(DroppedResourceClass, &GetOwningActor()->GetActorTransform());
	
	//Pawn Message
	GetOwningCharacter()->PawnDeath();

	//Collision
	GetOwningCharacter()->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	//Ragdoll
	if (Death_Config->bRagdoll)
	{
		if (ACharacter* Character = GetOwningCharacter())
		{
			if (USkeletalMeshComponent* SourceMesh = Character->GetMesh(); IsValid(SourceMesh))
			{
				if (ASkeletalMeshActor* RagdollActor = GetWorld()->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SourceMesh->GetComponentTransform()); IsValid(RagdollActor))
				{
					RagdollActor->GetSkeletalMeshComponent()->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
					RagdollActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
					
					if (Death_Config->AnimBlueprintClass)
					{
						FPoseSnapshot Snapshot {};
						SourceMesh->SnapshotPose(Snapshot);
						RagdollActor->GetSkeletalMeshComponent()->SetAnimClass(Death_Config->AnimBlueprintClass);
						if (UPoseAnimInstance* Instance = Cast<UPoseAnimInstance>(RagdollActor->GetSkeletalMeshComponent()->GetAnimInstance()))
							Instance->SetPose(Snapshot);
					}
					
					RagdollActor->GetSkeletalMeshComponent()->SetSimulatePhysics(true);
					RagdollActor->GetSkeletalMeshComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
					RagdollActor->GetSkeletalMeshComponent()->SetPhysicsBlendWeight(1.0f);
				}
			}
		}
	}

	//Destroy Character
	GetOwningCharacter()->Destroy();
}


    
