// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AnimDataController_FP.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Starfire/Animation/Sf_AnimHelper.h"
#include "Starfire/Sf_Bases/Sf_Character.h"
#include "Starfire/Shared/HSM/SF_CharacterStateMachine.h"
#include "Starfire/Shared/Weapon/StructsAndEnums/MeleeInfo.h"
#include "Sf_FP_Character.generated.h"

class USf_CoverGenComponent;
class UInteractComponent;
class USpringArmComponent;
class UCameraComponent;
class USf_Equipment;
class USf_FP_CharacterMovementComponent;
class UBoxComponent;

UCLASS(Config = Game,BlueprintType)
class STARFIRE_API ASf_FP_Character : public ASf_Character
{
	GENERATED_BODY()
	
	friend USf_FP_CharacterMovementComponent;

public:
	ASf_FP_Character(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Functions

public:
	FCollisionQueryParams GetIgnoreCharacterParams() const;

#pragma endregion

#pragma region Properties

protected:
	bool bCustomJumpPressed = false;
	
#pragma endregion

#pragma region Components
public:
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_FP_CharacterMovementComponent* GetSf_FP_CharacterMovement() const{return SFCharacterMovementComponent;};
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const{return FirstPersonCamera;};
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const{return FirstPersonMesh;};
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE UInteractComponent* GetInteractComponent() const{return InteractComponent;};
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_CharacterStateMachine* GetCharacterStateMachine() const{return SfCharacterStateMachine;};
	UFUNCTION(BlueprintPure,Category = "Character")
	FORCEINLINE USf_CoverGenComponent* GetCoverGenComponent() const{return SfCoverGenComponent;};
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USf_FP_CharacterMovementComponent* SFCharacterMovementComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(Category="Character", EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	UInteractComponent* InteractComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StateMachine")
	USf_CharacterStateMachine* SfCharacterStateMachine;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Animation")
	USf_AnimDataController_FP* WeaponAnimMontageController;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	USf_CoverGenComponent* SfCoverGenComponent;
	
#pragma endregion

#pragma region Editor
public:
#if WITH_EDITOR
	FReply OnSaveButtonClicked() const;
	FReply OnLoadButtonClicked() const;
#endif

	UPROPERTY(EditDefaultsOnly, meta = (GetOptions = "GetAllPropertiesWithoutCustomConfig"), Category = "CustomConfig")
	TArray<FName> AdditionalConfigSavedProperties = {};
#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetAllPropertiesWithoutCustomConfig() const;
#endif
	
	
#pragma endregion

#pragma region IWeaponOwnerInterface

	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual FTransform GetFireTransform_Implementation() const override;
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
	virtual USf_AnimHelper* GetAnimDataHelper_Implementation() const override;
#pragma endregion

#pragma region Reset
public:
	UFUNCTION(BlueprintCallable)
	void Respawn();

#pragma endregion
};
