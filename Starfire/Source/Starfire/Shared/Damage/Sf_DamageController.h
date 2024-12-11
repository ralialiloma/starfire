// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SF_Hitbox.h"
#include "Components/ActorComponent.h"
#include "Starfire/StarFireGameplayTags.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Sf_DamageController.generated.h"

#define APPLY_DAMAGE(DamageController, Damage, HitLocation, HitNormal, HitComponent, Type) \
(DamageController->ApplyDamage(Damage,HitLocation,HitNormal,HitComponent,Sf_GameplayTags::Gameplay::DamageType::Type))


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FReceivedDamage_BP , float, RemainingHealth, float, DamageReceived, FVector, HitLocation, FVector, 
HitNormal,FGameplayTag,DamageType);
DECLARE_MULTICAST_DELEGATE_FiveParams(FReceivedDamage_CPP , float, float, FVector, FVector, FGameplayTag);

UCLASS(ClassGroup=(Damage), meta=(BlueprintSpawnableComponent), Blueprintable)
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
	float ApplyDamage(
		float Damage,
		FVector HitLocation,
		FVector HitNormal,
		UPrimitiveComponent* HitComponent,
		UPARAM(meta=(Categories="Gameplay.DamageType"))FGameplayTag DamageType );
	UFUNCTION(BlueprintCallable)
	void Reset();
	UFUNCTION(BlueprintCallable)
	void Heal(float AmountOfHeal, bool bInternal = false);
	UFUNCTION(BlueprintCallable)
	float IsMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	float IsZeroHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthInPercent() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentArmor() const;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);
	
private:
	void PassiveHeal(float DeltaSeconds);

#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable, Category = "Damage Receiver")
	FReceivedDamage_BP OnDamageReceived_BP;
	FReceivedDamage_CPP OnDamageReceived_CPP;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnFullHealth_BP;
	FSf_VoidDelegate_CPP OnFullHealth_CPP;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnZeroHealth_BP;
	FSf_VoidDelegate_CPP OnZeroHealth_CPP;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnHeal_BP;
	FSf_VoidDelegate_CPP OnHeal_CPP;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FSf_VoidDelegate_BP OnHealthChanged_BP;
	FSf_VoidDelegate_CPP OnHealthChanged_CPP;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver", meta = (CustomConfig))
	bool bStartWithMaxHealth = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver", meta = (CustomConfig))
	float MaxHealth = 100;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver", meta = (CustomConfig))
	float MaxArmor = 50;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver", meta = (CustomConfig))
	bool bEnablePassiveHealing = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver",meta=(EditCondition="bEnablePassiveHealing", EditConditionHides,CustomConfig))
	float PassiveHealingRatePerSecond = 5;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver",meta=(EditCondition="bEnablePassiveHealing", EditConditionHides,CustomConfig))
	float PassiveHealingStartAfterDamageInSeconds = 5;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver",meta = (Categories = "Gameplay.DamageType",CustomConfig))
	FGameplayTagContainer SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::FirePlayer);
private:
	
	UPROPERTY()
	float CurrentHealth = 0;
	UPROPERTY()
	float CurrentArmor = 0;
	UPROPERTY()
	bool bShouldPassiveHeal;
	UPROPERTY()
	FTimerHandle PassiveHealCooldown;

#pragma endregion
	
		
};
