// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFeature.h"
#include "WeaponAnimMontageController_TP.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/DamageSystem/Sf_DamageController.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_NPCharacter.generated.h"


UCLASS(BlueprintType)
class STARFIRE_API ASf_NPCharacter : public ACharacter,public IWeaponOwner
{
	GENERATED_BODY()

	//Actor
public:
	ASf_NPCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma region Functions
public:
	//Components
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_DamageController* GetSfDamageController();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USF_Equipment* GetSfEquipment();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_TP_CharacterMovementComponent* GetSfMovement();

	//Collision
	FCollisionQueryParams GetIgnoreCharacterParams();
	TArray<AActor*> GetIgnoreActors();
#pragma endregion
	
#pragma region Properties

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	USF_Equipment* SfEquipmentComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	UBoxComponent* MeleeTransform;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Animation")
	USf_WeaponAnimMontageController_TP* WeaponAnimMontageController;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageController* DamageController;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USf_TP_CharacterMovementComponent* SFCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
#pragma endregion

#pragma region Features
	
#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSet<TSubclassOf<UEnemyFeature>> StartFeatures;
private:
	UPROPERTY()
	TSet<UEnemyFeature*> Features;

#pragma endregion

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);
	
	template <typename FeatureType>
	FeatureType* GetFeatureByClass();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeatures")
	bool TryAddFeature(TSubclassOf<UEnemyFeature>& FeatureType);
private:
	void ImportStartFeatures();
	TSet<TSubclassOf<UEnemyFeature>> GetAllStartFeatures() const;

#pragma endregion

#pragma endregion
	
#pragma region WeaponOwner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;

	virtual USf_WeaponAnimMontageController* GetAnimMontageController_Implementation() const override;
#pragma endregion

};

template <typename FeatureType>
FeatureType* ASf_NPCharacter::GetFeatureByClass()
{
	static_assert(TIsDerivedFrom<FeatureType, UEnemyFeature>::IsDerived, "FeatureType must be derived from UEnemyFeature");
	UEnemyFeature* FoundFeature =  GetFeatureByClass(FeatureType::StaticClass());
	if (!IsValid(FoundFeature))
		return nullptr;
	return Cast<FeatureType>(FoundFeature);
}

