// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFeature.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/DamageSystem/Sf_DamageReceiver.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Sf_NPCharacter.generated.h"

UCLASS(BlueprintType)
class STARFIRE_API ASf_NPCharacter : public ACharacter,public IWeaponOwner
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TArray<TSubclassOf<UEnemyFeature>> StartFeatures;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageReceiver* SfDamageReceiver;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	USF_Equipment* SfEquipmentComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	UBoxComponent* MeleeTransform;

	//Interface
public:
		//Features
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);

	UFUNCTION(BlueprintCallable, Category = "EnemyFeatures")
	bool TryAddFeature(TSubclassOf<UEnemyFeature>& FeatureType);

		//Components
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_DamageReceiver* GetSfDamageReceiver();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USF_Equipment* GetSfEquipment();

		//Collision
	FCollisionQueryParams GetIgnoreCharacterParams();
	TArray<AActor*> GetIgnoreActors();

		//Behaviour

	//Actor
public:
	ASf_NPCharacter();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Internal
private:

		//Features
	UPROPERTY()
	TArray<UEnemyFeature*> Features;

	UFUNCTION()
	void ImportStartFeatures();

	//Weapon Owner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
};
