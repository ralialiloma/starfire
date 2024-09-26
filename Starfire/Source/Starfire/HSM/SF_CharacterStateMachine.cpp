// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_CharacterStateMachine.h"
#include "GameFramework/Character.h"
#include "Starfire/Character/Sf_Character.h"
#include "Starfire/Utility/FunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(SF_CharacterStateMachine, Display, Display);

USf_CharacterStateMachine::USf_CharacterStateMachine(const FObjectInitializer& ObjectInitializer): StateCallstack(nullptr),
														StateDefinitionDT(nullptr),
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

void USf_CharacterStateMachine::SubscribeToInputActions(TArray<UInputAction*> InputActions)
{
	for (UInputAction* InputAction: InputActions)
	{
		UFunctionLibrary::BindInputAction<FRunCallStackByInputAction>(
			InputAction,
			this,
			"ActionName",
			IE_Pressed,
			this,
			&USf_CharacterStateMachine::RunCallStackByInputAction,
			InputAction
			);
	}
}

void USf_CharacterStateMachine::InitializeComponent()
{
	Super::InitializeComponent();
	OwningCharacter = Cast<ASf_Character>(GetOwner()) ;
	StateCallstack = nullptr;

	if (!IsValid(OwningCharacter))
	{
		UE_LOG(SF_CharacterStateMachine, Error, TEXT("This component only works with the %s"),*ASf_Character::StaticClass()->GetName());
		return;
	}

	if (!IsValid(StateDefinitionDT))
	{
		UE_LOG(
			SF_CharacterStateMachine,
			Error,
			TEXT("This component requires a valid state defintion in order to work"));
		return;
	}

	if (!IsValid(StateFeatureDefinitionDT))
	{
		UE_LOG(
			SF_CharacterStateMachine,
			Error,
			TEXT("This component requires a valid state feature defintion in order to work"));
		return;
	}

	StateCallstack = NewObject<UStateCallstack>(this);
	StateCallstack->OwningCharacter = OwningCharacter;
	StateCallstack->StateDefintionDT = StateDefinitionDT;
	StateCallstack->StateFeatureDefinitionDT = StateFeatureDefinitionDT;
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

