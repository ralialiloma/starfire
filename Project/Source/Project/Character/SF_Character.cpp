// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_Character.h"

#include "Movement/SF_CharacterMovementComponent.h"


ASf_Character::ASf_Character(const FObjectInitializer& ObjectInitializer)
	: Super(
		  ObjectInitializer.SetDefaultSubobjectClass<USF_CharacterMovementComponent>(
			  ACharacter::CharacterMovementComponentName))
{
	SFCharacterMovementComponent  = Cast<USF_CharacterMovementComponent>(GetCharacterMovement());;
}

void ASf_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SFCharacterMovementComponent = Cast<USF_CharacterMovementComponent>(GetCharacterMovement());
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

