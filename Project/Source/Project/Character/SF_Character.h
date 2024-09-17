// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SF_Character.generated.h"

UCLASS(Config = Game)
class PROJECT_API ASf_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	class USF_CharacterMovementComponent* SFCharacterMovementComponent;

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	
	FCollisionQueryParams GetIgnoreCharacterParams();

	UFUNCTION(BlueprintPure)
	FORCEINLINE USF_CharacterMovementComponent* GetSfCharacterMovementComponent() const{return SFCharacterMovementComponent;};

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
