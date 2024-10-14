// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SF_Hitbox.h"
#include "Components/ActorComponent.h"
#include "Sf_DamageController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReceivedDamage , float, RemainingHealth, float, DamageReceived, FVector, HitLocation, FVector, HitNormal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STARFIRE_API USf_DamageController : public UActorComponent
{
	GENERATED_BODY()

public:	
	USf_DamageController();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxHealth= 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxArmor = 50;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FReceivedDamage OnDamageReceived;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FZeroHealth OnZeroHealth;

private:

	UPROPERTY()
	float CurrentHealth = 0;

	UPROPERTY()
	float CurrentArmor = 0;

	//Interface
public:
	UFUNCTION(BlueprintCallable)
	float ApplyDamage(float Damage, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent);

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentArmor() const;

	//Actor Component
public:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Private
private:
	void ImportHitBoxes();

		
};
