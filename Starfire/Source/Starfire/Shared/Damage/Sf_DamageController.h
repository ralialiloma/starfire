// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SF_Hitbox.h"
#include "Components/ActorComponent.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Sf_DamageController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReceivedDamage , float, RemainingHealth, float, DamageReceived, FVector, HitLocation, FVector, HitNormal);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STARFIRE_API USf_DamageController : public UActorComponent
{
	GENERATED_BODY()

public:	
	USf_DamageController();
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float Damage, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent);

	UFUNCTION(BlueprintCallable)
	void Heal(float AmountOfHeal, bool bInternal = false);

	UFUNCTION(BlueprintCallable)
	void RestoreHealth();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentArmor() const;

#pragma endregion

#pragma region Properties
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxHealth= 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxArmor = 50;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float HealRatePerSecond = 5;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FReceivedDamage OnDamageReceived;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnZeroHealth_BP;
	FSf_VoidDelegate_CPP OnZeroHealth_CPP;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnHeal_BP;
	FSf_VoidDelegate_CPP OnHeal_CPP;
private:
	UPROPERTY()
	float CurrentHealth = 0;

	UPROPERTY()
	float CurrentArmor = 0;

	UPROPERTY()
	FTimerHandle PassiveHealTimer;

#pragma endregion
	
		
};
