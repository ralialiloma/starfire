// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Character.h"

#include "Sf_Equipment.h"
#include "WeaponAnimMontageController_FP.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Movement/Sf_FP_CharacterMovementComponent.h"
#include "Starfire/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Interact/InteractComponent.h"
#include "Starfire/Utility/ConfigLoader.h"
#include "UObject/UnrealTypePrivate.h"

DEFINE_LOG_CATEGORY_STATIC(SF_Character_Log, Display, Display);

TArray<FName> ASf_Character::GetAllPropertiesWithoutCustomConfig()
{
	TArray<FName> Returns;
	
	for (TFieldIterator<FProperty> PropIt(StaticClass()); PropIt; ++PropIt)
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

	return Returns;
}

FReply ASf_Character::OnSaveButtonClicked()
{
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject(),"SF_CharacterDefault");
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_FP_CharacterMovementComponent>(),"SF_CharacterDefault");
	FConfigLoader::SaveCustomConfig(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USF_Equipment>(),"USF_Equipment");
	return FReply::Handled();
}

FReply ASf_Character::OnLoadButtonClicked()
{
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject(),"SF_CharacterDefault");
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USf_FP_CharacterMovementComponent>(),"SF_CharacterDefault");
	FConfigLoader::LoadConfigFile(GetClass()->GetDefaultObject<AActor>()->FindComponentByClass<USF_Equipment>(),"USF_Equipment");
	return FReply::Handled();
}

ASf_Character::ASf_Character(const FObjectInitializer& ObjectInitializer)
	: Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USf_FP_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
		  .DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{

	//CharacterMovement
	SFCharacterMovementComponent  = Cast<USf_FP_CharacterMovementComponent>(GetCharacterMovement());;

	//SprintArmComponent
	SprintArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SprintArmComponent->SetUsingAbsoluteLocation(false);
	SprintArmComponent->SetUsingAbsoluteRotation(false);
	SprintArmComponent->SetUsingAbsoluteScale(false);
	SprintArmComponent->SetupAttachment(GetCapsuleComponent());

	//ProceduralCameraAnimationRoot
	ProceduralCameraAnimationRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ProcCameraAnimRoot"));
	ProceduralCameraAnimationRoot->SetUsingAbsoluteLocation(false);
	ProceduralCameraAnimationRoot->SetUsingAbsoluteRotation(false);
	ProceduralCameraAnimationRoot->SetUsingAbsoluteScale(false);
	ProceduralCameraAnimationRoot->SetupAttachment(SprintArmComponent);

	//FirstPersonCamera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCam"));
	FirstPersonCamera->SetUsingAbsoluteLocation(false);
	FirstPersonCamera->SetUsingAbsoluteRotation(false);
	FirstPersonCamera->SetUsingAbsoluteScale(false);
	FirstPersonCamera->SetupAttachment(ProceduralCameraAnimationRoot);

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

	//Interact
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact"));
	InteractComponent->SetupAttachment(RootComponent);

	//Character State Machine
	SFCharacterStateMachine =
		CreateDefaultSubobject<USf_CharacterStateMachine>(TEXT("StateMachine"));

	//Weapon Animation Receiver
	WeaponAnimMontageController = CreateDefaultSubobject<USf_WeaponAnimMontageController_FP>(TEXT("WeaponAnimMontageController"));

	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));

	//AIPerceptionStimuliSourceComponent
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
}

void ASf_Character::PostInitProperties()
{
	Super::PostInitProperties();
}

void ASf_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SFCharacterMovementComponent = Cast<USf_FP_CharacterMovementComponent>(GetCharacterMovement());
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
	Params.AddIgnoredActor(SFEquipmentComponent->GetActiveWeapon());

	return Params;
}

// Called when the game starts or when spawned
void ASf_Character::BeginPlay()
{
	Super::BeginPlay();
	//LoadConfig();
}

void ASf_Character::Jump()
{
	Super::Jump();

	bCustomJumpPressed = true;
	bPressedJump = false;
}

void ASf_Character::StopJumping()
{
	Super::StopJumping();

	bCustomJumpPressed = false;
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

USf_WeaponAnimMontageController* ASf_Character::GetAnimMontageController_Implementation() const
{
	return WeaponAnimMontageController;
}


