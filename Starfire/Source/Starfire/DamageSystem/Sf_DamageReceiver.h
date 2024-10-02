// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SF_Hitbox.h"
#include "Components/ActorComponent.h"
#include "Sf_DamageReceiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReceivedDamage , float, RemainingHealth, float, DamageReceived, FVector, HitLocation, FVector, HitNormal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STARFIRE_API USf_DamageReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	USf_DamageReceiver();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage Receiver")
	float MaxArmor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FReceivedDamage OnDamageReceived;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,BlueprintAssignable, Category = "Damage Receiver")
	FZeroHealth OnZeroHealth;

private:

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float CurrentArmor;

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
