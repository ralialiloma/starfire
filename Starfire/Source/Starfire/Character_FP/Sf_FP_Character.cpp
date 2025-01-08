// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_FP_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Sf_FP_CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/Core/Sf_GameState.h"
#include "Starfire/Shared/Interact/InteractComponent.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Starfire/Utility/ConfigLoader.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Character_Log, Display, Display);
#if WITH_EDITOR
TArray<FName> ASf_FP_Character::GetAllPropertiesWithoutCustomConfig() const
{
	TArray<FName> Returns;
	
	for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		if (!Property)
			continue;

		if (Property->HasMetaData(TEXT("CustomConfig")))
			continue;
		
		if (!Property->HasAnyPropertyFlags(CPF_Edit | CPF_EditConst | CPF_InstancedReference))
			continue;
		
		Returns.Add(Property->GetFName());
	}

	TSet<UActorComponent*> ActorComponents = GetClass()->GetDefaultObject<ASf_FP_Character>()->GetComponents();

	for(UActorComponent* Component:ActorComponents)
	{
		for (TFieldIterator<FProperty> PropIt(Component->GetClass()); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;

			if (!Property)
				continue;

			if (Property->HasMetaData(TEXT("CustomConfig")))
				continue;
		
			if (!Property->HasAnyPropertyFlags(CPF_Edit | CPF_EditConst | CPF_InstancedReference))
				continue;
		
			Returns.Add(Property->GetFName());
		}
	}

	return Returns;
}

FReply ASf_FP_Character::OnSaveButtonClicked() const
{
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject(),"SF_CharacterDefault");
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_FP_CharacterMovementComponent>(),"SF_CharacterDefault");
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_Equipment>(),"USf_Equipment");
	return FReply::Handled();
}

FReply ASf_FP_Character::OnLoadButtonClicked() const
{
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject(),"SF_CharacterDefault");
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_FP_CharacterMovementComponent>(),"SF_CharacterDefault");
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_Equipment>(),"USf_Equipment");
	return FReply::Handled();
}
#endif

ASf_FP_Character::ASf_FP_Character(const FObjectInitializer& ObjectInitializer)
	: Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USf_FP_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
		  .DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{

	//CharacterMovement
	SFCharacterMovementComponent  = Cast<USf_FP_CharacterMovementComponent>(GetCharacterMovement());;

	//SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetUsingAbsoluteLocation(false);
	SpringArmComponent->SetUsingAbsoluteRotation(false);
	SpringArmComponent->SetUsingAbsoluteScale(false);
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());

	//FirstPersonCamera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCam"));
	FirstPersonCamera->SetUsingAbsoluteLocation(false);
	FirstPersonCamera->SetUsingAbsoluteRotation(false);
	FirstPersonCamera->SetUsingAbsoluteScale(false);
	FirstPersonCamera->SetupAttachment(SpringArmComponent);

	//First Person Person Character Mesh
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FirstPersonMesh->SetUsingAbsoluteLocation(false);
	FirstPersonMesh->SetUsingAbsoluteRotation(false);
	FirstPersonMesh->SetUsingAbsoluteScale(false);
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetRelativeLocation(FVector(25,0,150));
	
	SfEquipmentComponent->SetupAttachment(FirstPersonMesh,TEXT("GripPoint"));
	

	//Interact
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact"));
	InteractComponent->SetupAttachment(RootComponent);

	//Character State Machine
	SfCharacterStateMachine =
		CreateDefaultSubobject<USf_CharacterStateMachine>(TEXT("StateMachine"));

	//Weapon Animation Receiver
	WeaponAnimMontageController = CreateDefaultSubobject<USf_AnimDataController_FP>(TEXT("WeaponAnimMontageController"));

	//AIPerceptionStimuliSourceComponent
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
}

void ASf_FP_Character::PostInitProperties()
{
	Super::PostInitProperties();
}

void ASf_FP_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SFCharacterMovementComponent = Cast<USf_FP_CharacterMovementComponent>(GetCharacterMovement());
	SfDamageController->OnZeroHealth_CPP.AddLambda(
		[this]()->void
		{
			OnPawnDeath_CPP.Broadcast();
			OnPawnDeath_BP.Broadcast();
		});
	//FConfigLoader::LoadConfigFile<ASf_FP_Character>(this,"SF_CharacterDefault");
}

void ASf_FP_Character::ReceiveKnockback_Implementation(FVector KnockbackForce)
{
	IKnockbackReceiver::ReceiveKnockback_Implementation(KnockbackForce);
	LaunchCharacter(KnockbackForce,true,false);
}
#if WITH_EDITOR
void ASf_FP_Character::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


#endif

FCollisionQueryParams ASf_FP_Character::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(SfEquipmentComponent->GetActiveWeapon());
	return Params;
}

void ASf_FP_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ASf_FP_Character::Jump()
{
	Super::Jump();

	bCustomJumpPressed = true;
	bPressedJump = false;
	bCustomJumpDown = true;
}

void ASf_FP_Character::StopJumping()
{
	Super::StopJumping();

	bCustomJumpPressed = false;
	bCustomJumpDown = false;
}

void ASf_FP_Character::CheckJumpInput(float DeltaTime)
{
	JumpCurrentCountPreJump = JumpCurrentCount;

	if (SFCharacterMovementComponent)
	{
		if (bPressedJump)
		{
			// If this is the first jump and we're already falling,
			// then increment the JumpCount to compensate.
			const bool bFirstJump = JumpCurrentCount == 0;
			if (bFirstJump && SFCharacterMovementComponent->IsFalling() && !IsInJumpAllowance())
			{
				JumpCurrentCount++;
			}

			const bool bDidJump = CanJump() && SFCharacterMovementComponent->DoJump(bClientUpdating);
			if (bDidJump)
			{
				// Transition from not (actively) jumping to jumping.
				if (!bWasJumping)
				{
					JumpCurrentCount++;
					JumpForceTimeRemaining = GetJumpMaxHoldTime();
					OnJumped();
				}
			}

			bWasJumping = bDidJump;
		}
	}
}

float ASf_FP_Character::GetJumpMaxHoldTime() const
{
	if (SFCharacterMovementComponent->PreviousMovementMode == MOVE_Custom && SFCharacterMovementComponent->PreviousCustomMode == CMOVE_WallRun)
		return 0;
	return Super::GetJumpMaxHoldTime();
}

void ASf_FP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASf_FP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ASf_FP_Character::IsInJumpAllowance() const
{
	return (SFCharacterMovementComponent->ElapsedInAirJumpAllowance <= SFCharacterMovementComponent->InAirJumpAllowance);
}

bool ASf_FP_Character::CanJumpInternal_Implementation() const
{
	// Ensure that the CharacterMovement state is valid
	bool bJumpIsAllowed = GetCharacterMovement()->CanAttemptJump();

	if (bJumpIsAllowed)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			if ((JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling()) &&
				!IsInJumpAllowance())
			{
				bJumpIsAllowed = JumpCurrentCount + 1 < JumpMaxCount;
			}
			else
			{
				bJumpIsAllowed = JumpCurrentCount < JumpMaxCount;
			}
		}
		else
		{
			// Only consider JumpKeyHoldTime as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bJumpIsAllowed = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}

	return bJumpIsAllowed;
}

UAnimInstance* ASf_FP_Character::GetCharacterAnimInstance_Implementation() const
{
	return FirstPersonMesh->GetAnimInstance();
}

FTransform ASf_FP_Character::GetFireTransform_Implementation() const
{
	return FirstPersonCamera->GetComponentTransform();
}

FMeleeInfo ASf_FP_Character::GetMeleeInfo_Implementation() const
{
	const FMeleeInfo MeleeInfo = FMeleeInfo(WeaponMeleeTransform);
	return MeleeInfo;
}

USf_AnimHelper* ASf_FP_Character::GetAnimDataHelper_Implementation() const
{
	return WeaponAnimMontageController;
}

void ASf_FP_Character::Respawn()
{
	GetMovementComponent()->StopMovementImmediately();

	if (ASf_GameState* GameState = GetWorld()->GetGameState<ASf_GameState>())
	{
		FTransform SpawnTransform = FTransform();
		
		if (!GameState->GetLastCheckpointTransform(SpawnTransform))
		{
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);
			
			float ClosestDistance = MAX_FLT;
			for (AActor* FoundActor : OutActors)
			{
				float CurrentDistance = FVector::Dist(GetActorLocation(), FoundActor->GetActorLocation());

				if (CurrentDistance < ClosestDistance)
				{
					ClosestDistance = CurrentDistance;
					SpawnTransform = FoundActor->GetTransform();
				}
			}
		}

		GetController<APlayerController>()->SetControlRotation(SpawnTransform.Rotator());
		SetActorTransform(SpawnTransform);
		UFXSubsystem::Get()->PlayFX(Sf_GameplayTags::Effects::Messages::FP::Respawn);
		GetSfDamageController()->Reset();
		
	}
}