// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateMachine.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UCharacterStateMachine::UCharacterStateMachine()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCharacterStateMachine::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
	return StateCallstack->TryAddState(BaseStateClass, FStateModuleDataStruct(Cast<ACharacter>(GetOwner())));
}

bool UCharacterStateMachine::TryRemoveState(TSubclassOf<UBaseState> BaseStateClass)
{
	return StateCallstack->TryRemoveState(BaseStateClass, FStateModuleDataStruct(Cast<ACharacter>(GetOwner())));
}


void UCharacterStateMachine::BeginPlay()
{
	Super::BeginPlay();
	 //StateCallstack = NewObject<UStateCallstack>(this);
}


void UCharacterStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

