// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_CharacterStateMachine.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(SF_CharacterStateMachine, Display, Display);

USf_CharacterStateMachine::USf_CharacterStateMachine(const FObjectInitializer& ObjectInitializer): StateCallstack(nullptr),
														StateDefinitions(nullptr),
                                                        OwningCharacter(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent  = true;
}

bool USf_CharacterStateMachine::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
	return StateCallstack->TryAddState(BaseStateClass);
}

bool USf_CharacterStateMachine::TryRemoveState(TSubclassOf<UBaseState> BaseStateClass)
{
	return StateCallstack->TryRemoveState(BaseStateClass);
}

void USf_CharacterStateMachine::InitializeComponent()
{
	Super::InitializeComponent();
	OwningCharacter = Cast<ACharacter>(GetOwner()) ;
	StateCallstack = nullptr;

	if (!IsValid(OwningCharacter))
	{
		UE_LOG(SF_CharacterStateMachine, Error, TEXT("This component only works with characters"));
		return;
	}

	if (!IsValid(StateDefinitions))
	{
		UE_LOG(
			SF_CharacterStateMachine,
			Error,
			TEXT("This component requires a valid state defintion in order to work"));
		return;
	}

	StateCallstack = NewObject<UStateCallstack>(this);
	StateCallstack->OwningCharacter = OwningCharacter;
	StateCallstack->StateDefintions = StateDefinitions;
}


void USf_CharacterStateMachine::BeginPlay()
{
	Super::BeginPlay();
	 //StateCallstack = NewObject<UStateCallstack>(this);
}


void USf_CharacterStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

