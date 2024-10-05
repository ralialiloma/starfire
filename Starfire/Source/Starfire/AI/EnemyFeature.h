// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Starfire/Utility/BetterObject.h"
#include "EnemyFeature.generated.h"

class ASf_NPCharacter;

UCLASS(Blueprintable)
class STARFIRE_API UEnemyFeature : public UBetterObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool RunInTick = false;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	float UpdateRateInSeconds = 1;

protected:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	ASf_NPCharacter* OwningAIHolder = nullptr;

public:
	void Initialize(ASf_NPCharacter* Holder);

	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value);

	// For Int Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value);

	// For Bool Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value);

	// For Actor Blackboard Keys
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardActorValue(EActorBlackboardKey ActorBlackboardKey, AActor* Value);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	UCharacterMovementComponent* GetCharacterMovement();
	
};
