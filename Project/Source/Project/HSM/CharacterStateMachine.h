// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateCallstack.h"
#include "CharacterStateMachine.generated.h"


//Define Delegate
/*template <typename T>
;*/

UCLASS(Blueprintable, ClassGroup=(StateMachine), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UCharacterStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStateMachine();

	UFUNCTION(BlueprintCallable)
	bool TryAddState(TSubclassOf<UBaseState> BaseStateClass);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveState(TSubclassOf<UBaseState> BaseStateClass);
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStateCallstack* StateCallstack;
		
};
