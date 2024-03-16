// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStateMachine.h"


// Sets default values for this component's properties
UCharacterStateMachine::UCharacterStateMachine()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UCharacterStateMachine::BeginPlay()
{
	Super::BeginPlay();
	StateCallstack = NewObject<UStateCallstack>(this);
	


	/*InterfaceAction JumpAction;
	JumpAction.BindLambda([](IJumpInterface* Object){Object->JumpAction();});
	
	StateCallstack->RunCallStack<IJumpInterface>(JumpAction);*/
}


void UCharacterStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

