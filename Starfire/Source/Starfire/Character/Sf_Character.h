// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimMontageController_FP.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Starfire/Animation/WeaponAnimMontageController.h"
#include "Starfire/DamageSystem/Sf_DamageController.h"
#include "Starfire/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_Character.generated.h"

class UInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class USF_Equipment;
class USf_FP_CharacterMovementComponent;
class UBoxComponent;

UCLASS(Config = Game,BlueprintType)
class STARFIRE_API ASf_Character : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USf_FP_CharacterMovementComponent* SFCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	USF_Equipment* SFEquipmentComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	USceneComponent* ProceduralCameraAnimationRoot;
	
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Animation")
	USf_WeaponAnimMontageController_FP* WeaponAnimMontageController;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageController* DamageController;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;


public:
#pragma region Editor
#if WITH_EDITOR

	FReply OnSaveButtonClicked();
	FReply OnLoadButtonClicked();
	
#endif
#pragma endregion

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	FCollisionQueryParams GetIgnoreCharacterParams();

	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_FP_CharacterMovementComponent* GetSfCharacterMovementComponent() const{return SFCharacterMovementComponent;};

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

public:
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	bool bCustomJumpPressed = false;


	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//IWeaponOwner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;

	virtual USf_WeaponAnimMontageController* GetAnimMontageController_Implementation() const override;
};
