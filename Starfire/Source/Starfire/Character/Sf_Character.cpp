// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Character.h"

#include "Sf_Equipment.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Movement/SF_CharacterMovementComponent.h"
#include "Starfire/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Interact/InteractComponent.h"
#include "Starfire/Utility/ConfigLoader.h"
#include "UObject/UnrealTypePrivate.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Character, Display, Display);

void ASf_Character::SaveToConfig()
{
	FConfigLoader::SaveCustomConfig<ASf_Character>(this,"SF_CharacterDefault");
	FConfigLoader::SaveCustomConfig<USF_CharacterMovementComponent>(SFCharacterMovementComponent,"SF_CharacterDefault");
}

void ASf_Character::LoadConfig()
{
	FConfigLoader::LoadConfigFile<ASf_Character>(this,"SF_CharacterDefault");
	FConfigLoader::LoadConfigFile<USF_CharacterMovementComponent>(SFCharacterMovementComponent,"SF_CharacterDefault");
}

ASf_Character::ASf_Character(const FObjectInitializer& ObjectInitializer)
	: Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USF_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
		  .DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{

	//CharacterMovement
	SFCharacterMovementComponent  = Cast<USF_CharacterMovementComponent>(GetCharacterMovement());;

	//SprintArmComponent
	SprintArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SprintArmComponent->SetUsingAbsoluteLocation(false);
	SprintArmComponent->SetUsingAbsoluteRotation(false);
	SprintArmComponent->SetUsingAbsoluteScale(false);
	SprintArmComponent->SetupAttachment(GetCapsuleComponent());

	//FirstPersonCamera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCam"));
	FirstPersonCamera->SetUsingAbsoluteLocation(false);
	FirstPersonCamera->SetUsingAbsoluteRotation(false);
	FirstPersonCamera->SetUsingAbsoluteScale(false);
	FirstPersonCamera->SetupAttachment(SprintArmComponent);

	//First Person Person Character Mesh
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FirstPersonMesh->SetUsingAbsoluteLocation(false);
	FirstPersonMesh->SetUsingAbsoluteRotation(false);
	FirstPersonMesh->SetUsingAbsoluteScale(false);
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetRelativeLocation(FVector(25,0,150));
	
	//Equipment
	SFEquipmentComponent = CreateDefaultSubobject<USF_Equipment>(TEXT("SFEquip"));
	SFEquipmentComponent->SetUsingAbsoluteLocation(false);
	SFEquipmentComponent->SetUsingAbsoluteRotation(false);
	SFEquipmentComponent->SetUsingAbsoluteScale(false);
	SFEquipmentComponent->SetupAttachment(FirstPersonMesh,TEXT("GripPoint"));


	//Weapon Transform
	MeleeTransform = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Transform"));
	MeleeTransform->SetUsingAbsoluteLocation(false);
	MeleeTransform->SetUsingAbsoluteRotation(false);
	MeleeTransform->SetUsingAbsoluteScale(false);
	MeleeTransform->SetupAttachment(FirstPersonCamera);
	MeleeTransform->SetRelativeLocation(FVector(30,0,0));
	MeleeTransform->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeleeTransform->ShapeColor = FColor::Purple;
	MeleeTransform->SetLineThickness(1.0f);

	////MeleeTransform->SetUsingAbsoluteRotation(false);
	//MeleeTransform->SetUsingAbsoluteScale(false);

	//Interact
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact"));
	InteractComponent->SetupAttachment(RootComponent);

	//Character State Machine
	SFCharacterStateMachine =
		CreateDefaultSubobject<USf_CharacterStateMachine>(TEXT("StateMachine"));

	//FConfigLoader::LoadConfigFile<ASf_Character>(this,"SF_CharacterDefault");
}

void ASf_Character::PostInitProperties()
{
	Super::PostInitProperties();
}

void ASf_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SFCharacterMovementComponent = Cast<USF_CharacterMovementComponent>(GetCharacterMovement());
	//FConfigLoader::LoadConfigFile<ASf_Character>(this,"SF_CharacterDefault");
}

void ASf_Character::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

FCollisionQueryParams ASf_Character::GetIgnoreCharacterParams()
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

// Called when the game starts or when spawned
void ASf_Character::BeginPlay()
{
	Super::BeginPlay();
	//LoadConfig();
}

// Called every frame
void ASf_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASf_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAnimInstance* ASf_Character::GetCharacterAnimInstance_Implementation() const
{
	return FirstPersonMesh->GetAnimInstance();
}

FTransform ASf_Character::GetFireTransform_Implementation() const
{
	return FirstPersonCamera->GetComponentTransform();
}

FMeleeInfo ASf_Character::GetMeleeInfo_Implementation() const
{
	const FVector Location = MeleeTransform->GetComponentLocation();
	const FVector Extent = MeleeTransform->GetScaledBoxExtent();
	const FRotator Rotation = MeleeTransform->GetComponentRotation();
	const FVector Direction = GetActorLocation()-Location;
	const FMeleeInfo MeleeInfo = FMeleeInfo(Location,Extent,Rotation,Direction);
	return MeleeInfo;
}


