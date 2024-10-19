// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sf_Equipment.h"
#include "WeaponAnimMontageController_FP.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Starfire/Animation/WeaponAnimMontageController.h"
#include "Starfire/DamageSystem/Sf_DamageController.h"
#include "Starfire/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Sf_Bases/Sf_CharacterBase.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_Character.generated.h"

class USf_CoverGenComponent;
class UInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class USF_Equipment;
class USf_FP_CharacterMovementComponent;
class UBoxComponent;

UCLASS(Config = Game,BlueprintType)
class STARFIRE_API ASf_Character : public ASf_CharacterBase
{
	GENERATED_BODY()
	
	friend USf_FP_CharacterMovementComponent;

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#pragma region Functions

public:
	FCollisionQueryParams GetIgnoreCharacterParams();

#pragma endregion

#pragma region Properties

protected:
	bool bCustomJumpPressed = false;
	
#pragma endregion

#pragma region Components
public:
	
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
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_CoverGenComponent* GetCoverGenComponent() const{return SfCoverGenComponent;};
	
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USf_FP_CharacterMovementComponent* SFCharacterMovementComponent;

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Animation")
	USf_WeaponAnimMontageController_FP* WeaponAnimMontageController;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageController* DamageController;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	USf_CoverGenComponent* SfCoverGenComponent;
	
#pragma endregion

#pragma region Editor
public:
#if WITH_EDITOR

	FReply OnSaveButtonClicked();
	FReply OnLoadButtonClicked();
	
#endif

	UPROPERTY(EditDefaultsOnly, meta = (GetOptions = "GetAllPropertiesWithoutCustomConfig"))
	TArray<FName> AdditionalConfigSavedProperties = {};

	UFUNCTION()
	static TArray<FName> GetAllPropertiesWithoutCustomConfig();
	
#pragma endregion

#pragma region IWeaponOwnerInterface

	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual FTransform GetFireTransform_Implementation() const override;
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
	virtual USf_WeaponAnimMontageController* GetAnimMontageController_Implementation() const override;

#pragma endregion
};
