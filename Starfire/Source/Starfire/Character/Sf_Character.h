// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Starfire/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_Character.generated.h"

class UInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class USF_Equipment;
class USF_CharacterMovementComponent;
class UBoxComponent;

UCLASS(Config = Game,BlueprintType)
class STARFIRE_API ASf_Character : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USF_CharacterMovementComponent* SFCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	USF_Equipment* SFEquipmentComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	USpringArmComponent* SprintArmComponent;
	
	UPROPERTY(Category="Character", EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	UInteractComponent* InteractComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StateMachine")
	USf_CharacterStateMachine* SFCharacterStateMachine;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	UBoxComponent* MeleeTransform;

public:

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Config")
	void SaveToConfig();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Config")
	void LoadConfig();

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
	FORCEINLINE UInteractComponent* GetInteractComponent() const{return InteractComponent;};

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
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
};
