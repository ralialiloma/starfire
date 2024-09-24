// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageReceiver.generated.h"

USTRUCT(BlueprintType)
struct STARFIRE_API FDamageAreaData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float MaxDistanceToBone;

	UPROPERTY(BlueprintReadWrite)
	float DamageMultiplier;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STARFIRE_API UDamageReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageReceiver();

	UPROPERTY()
    float Health;

	UPROPERTY()
	float Armor;

public:
	void ApplyDamage(float Damage, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float CalculateDamageMultiplier(FVector AreaLocation, FVector HitPosition, float DamageMultiplier, float MaxDistanceToBone);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
