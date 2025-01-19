#include "CF_Death.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Animation/SkeletalMeshActor.h"
#include "Starfire/Character_TP/PoseAnimInstance.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Features/Combat/CF_Combat.h"
#include "Starfire/Character_TP/Features/HitReact/CF_HitReact.h"
#include "Starfire/Character_TP/Features/Locomotion/CF_Locomotion.h"
#include "Starfire/Shared/ActionLogger/ActionLogger.h"
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
	if (Death_Config->bRagdoll && Death_Config->SpawnedRagdollActor)
	{
		if (ACharacter* Character = GetOwningCharacter())
		{
			if (USkeletalMeshComponent* SourceMesh = Character->GetMesh(); IsValid(SourceMesh))
			{
				if (AActor* Ragdoll = GetWorld()->SpawnActor<AActor>(Death_Config->SpawnedRagdollActor, SourceMesh->GetComponentTransform()); IsValid(Ragdoll))
				{
					USkeletalMeshComponent* Component = Ragdoll->GetComponentByClass<USkeletalMeshComponent>();
					if (!Component)
						return;
					
					Component->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
					Component->SetAnimationMode(EAnimationMode::AnimationBlueprint);
					
					FPoseSnapshot Snapshot {};
					SourceMesh->SnapshotPose(Snapshot);
					if (UPoseAnimInstance* Instance = Cast<UPoseAnimInstance>(Component->GetAnimInstance()))
					{
						Instance->SetPose(Snapshot);
						
						Component->RefreshBoneTransforms();
						Component->UpdateComponentToWorld();
						Component->FinalizeBoneTransform();   
					}
					
					Component->SetSimulatePhysics(true);
					Component->SetCollisionProfileName(TEXT("Ragdoll"));

					if (UCF_HitReact* HitReact = GetOwningCharacter()->GetFeatureByClass<UCF_HitReact>())
					{
						Component->AddImpulseAtLocation((-HitReact->LastHitDirection) * Death_Config->RagdollImpulse, HitReact->LastHitLocation, 
						HitReact->LastHitBone);
					}
				}
			}
		}
	}

	if (UActionLoggerSubSystem* SubSystem = UActionLoggerSubSystem::Get(GetWorld()))
		SubSystem->ReportAction(FActionLog(Sf_GameplayTags::Gameplay::ActionLogger::TP::Kill));

	//Destroy Character
	GetOwningCharacter()->Destroy();
}