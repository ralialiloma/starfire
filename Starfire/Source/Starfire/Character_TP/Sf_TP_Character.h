// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WeaponAnimMontageController_TP.h"
#include "Starfire/Sf_Bases/Sf_CharacterFeature.h"
#include "Perception/PawnSensingComponent.h"
#include "Starfire/Sf_Bases/Sf_Character.h"
#include "Sf_TP_Character.generated.h"


UCLASS(Config = Game,BlueprintType)
class STARFIRE_API ASf_TP_Character : public ASf_Character
{
	GENERATED_BODY()

	//Actor
public:
	ASf_TP_Character(const FObjectInitializer& ObjectInitializer);
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma region Functions
public:
	//Components
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_Equipment* GetSfEquipment();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_TP_CharacterMovementComponent* GetSf_TP_CharacterMovement();
	//Collision
	FCollisionQueryParams GetIgnoreCharacterParams();
	TArray<AActor*> GetIgnoreActors();
#pragma endregion
	
#pragma region Properties

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Animation")
	USf_WeaponAnimMontageController_TP* WeaponAnimMontageController;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	USf_TP_CharacterMovementComponent* SFCharacterMovementComponent;

#pragma endregion

#pragma region Features
	
#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSet<TSubclassOf<USf_CharacterFeature>> StartFeatures;
private:
	UPROPERTY()
	TSet<USf_CharacterFeature*> Features;

#pragma endregion

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	USf_CharacterFeature* GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class);
	
	template <typename FeatureType>
	FeatureType* GetFeatureByClass();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeatures")
	bool TryAddFeature(TSubclassOf<USf_CharacterFeature>& FeatureType);
private:
	void ImportStartFeatures();
	TSet<TSubclassOf<USf_CharacterFeature>> GetAllStartFeatures() const;

#pragma endregion

#pragma endregion
	
#pragma region WeaponOwner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	virtual FTransform GetFireTransform_Implementation() const override;
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
	virtual USf_AnimDataController* GetAnimMontageController_Implementation() const override;
#pragma endregion

};

template <typename FeatureType>
FeatureType* ASf_TP_Character::GetFeatureByClass()
{
	static_assert(TIsDerivedFrom<FeatureType, USf_CharacterFeature>::IsDerived, "FeatureType must be derived from USf_CharacterFeature");
	USf_CharacterFeature* FoundFeature =  GetFeatureByClass(FeatureType::StaticClass());
	if (!IsValid(FoundFeature))
		return nullptr;
	return Cast<FeatureType>(FoundFeature);
}

