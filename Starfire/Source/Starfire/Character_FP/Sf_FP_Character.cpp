// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_FP_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Sf_FP_CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_CoverGenComponent.h"
#include "Starfire/Shared/Interact/InteractComponent.h"
#include "Starfire/Utility/ConfigLoader.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Character_Log, Display, Display);

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
	
	
	//Equipment
	/*SfEquipmentComponent = CreateDefaultSubobject<USf_Equipment>(TEXT("SFEquip"));
	SfEquipmentComponent->SetUsingAbsoluteLocation(false);
	SfEquipmentComponent->SetUsingAbsoluteRotation(false);
	SfEquipmentComponent->SetUsingAbsoluteScale(false);*/
	SfEquipmentComponent->SetupAttachment(FirstPersonMesh,TEXT("GripPoint"));
	
	//Weapon Transform
	/*MeleeTransform = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Transform"));
	MeleeTransform->SetUsingAbsoluteLocation(false);
	MeleeTransform->SetUsingAbsoluteRotation(false);
	MeleeTransform->SetUsingAbsoluteScale(false);
	MeleeTransform->SetupAttachment(FirstPersonCamera);
	MeleeTransform->SetRelativeLocation(FVector(30,0,0));
	MeleeTransform->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeleeTransform->ShapeColor = FColor::Purple;
	MeleeTransform->SetLineThickness(1.0f);*/

	//Interact
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact"));
	InteractComponent->SetupAttachment(RootComponent);

	//Character State Machine
	SfCharacterStateMachine =
		CreateDefaultSubobject<USf_CharacterStateMachine>(TEXT("StateMachine"));

	//Weapon Animation Receiver
	WeaponAnimMontageController = CreateDefaultSubobject<USf_WeaponAnimMontageController_FP>(TEXT("WeaponAnimMontageController"));

	//AIPerceptionStimuliSourceComponent
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));

	//CoverGenComponent
	SfCoverGenComponent = CreateDefaultSubobject<USf_CoverGenComponent>(TEXT("CoverGenComponent"));
}

void ASf_FP_Character::PostInitProperties()
{
	Super::PostInitProperties();
}

void ASf_FP_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SFCharacterMovementComponent = Cast<USf_FP_CharacterMovementComponent>(GetCharacterMovement());
	//FConfigLoader::LoadConfigFile<ASf_FP_Character>(this,"SF_CharacterDefault");
}

void ASf_FP_Character::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

FCollisionQueryParams ASf_FP_Character::GetIgnoreCharacterParams()
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(SfEquipmentComponent->GetActiveWeapon());

	return Params;
}

// Called when the game starts or when spawned
void ASf_FP_Character::BeginPlay()
{
	Super::BeginPlay();
	//LoadConfig();
}

void ASf_FP_Character::Jump()
{
	Super::Jump();

	bCustomJumpPressed = true;
	bPressedJump = false;
}

void ASf_FP_Character::StopJumping()
{
	Super::StopJumping();

	bCustomJumpPressed = false;
}

// Called every frame
void ASf_FP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASf_FP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
	const FVector Location = MeleeTransform->GetComponentLocation();
	const FVector Extent = MeleeTransform->GetScaledBoxExtent();
	const FRotator Rotation = MeleeTransform->GetComponentRotation();
	const FVector Direction = GetActorLocation()-Location;
	const FMeleeInfo MeleeInfo = FMeleeInfo(Location,Extent,Rotation,Direction);
	return MeleeInfo;
}

USf_WeaponAnimMontageController* ASf_FP_Character::GetAnimMontageController_Implementation() const
{
	return WeaponAnimMontageController;
}


