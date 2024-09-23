// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project/HSM/SF_CharacterStateMachine.h"
#include "Project/Weapon/WeaponOwner.h"
#include "Sf_Character.generated.h"

UCLASS(Config = Game)
class PROJECT_API ASf_Character : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	class USF_CharacterMovementComponent* SFCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	class USF_Equipment* SFEquipmentComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	class USpringArmComponent* SprintArmComponent;
	
	UPROPERTY(Category="Character", EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	class UInteractBase* InteractComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StateMachine")
	USf_CharacterStateMachine* SFCharacterStateMachine;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StateMachine", meta = (CustomConfig))
	float TestValue;

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	FCollisionQueryParams GetIgnoreCharacterParams();

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USF_CharacterMovementComponent* GetSfCharacterMovementComponent() const{return SFCharacterMovementComponent;};

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const{return FirstPersonCamera;};

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const{return FirstPersonMesh;};

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USF_Equipment* GetSFEquipmentComponent() const{return SFEquipmentComponent;};

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE UInteractBase* GetInteractComponent() const{return InteractComponent;};

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_CharacterStateMachine* GetCharacterStateMachine() const{return SFCharacterStateMachine;};

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//IWeaponOwner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	
	virtual UCameraComponent* GetCamera_Implementation() const override;
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;

	virtual void GetWeaponAttachmentData_Implementation(FName& SocketName, USkeletalMeshComponent* SkeletalMeshComponent) const override;
};
