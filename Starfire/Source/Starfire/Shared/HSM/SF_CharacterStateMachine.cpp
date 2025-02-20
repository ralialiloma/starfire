// Fill out your copyright notice in the Description page of Project Settings.


#include "SF_CharacterStateMachine.h"
#include "GameFramework/Character.h"
#include "Starfire/Character_FP/Sf_FP_Character.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"



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

void USf_CharacterStateMachine::InitializeComponent()
{
	Super::InitializeComponent();
	OwningCharacter = Cast<ASf_FP_Character>(GetOwner()) ;
	StateCallstack = nullptr;

	if (!IsValid(OwningCharacter))
	{
		UE_LOG(SF_CharacterStateMachine, Error, TEXT("This component only works with the %s"),*ASf_FP_Character::StaticClass()->GetName());
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

	StateCallstack = NewObject<UStateCallstack>(this);
	StateCallstack->OwningCharacter = OwningCharacter;
	StateCallstack->StateDefintionDT = StateDefinitionDT;
}


void USf_CharacterStateMachine::BeginPlay()
{
	Super::BeginPlay();
}


void USf_CharacterStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::HSM::Name,EDebugVisualType::Print) && IsValid(StateCallstack))
	{
		StateCallstack->DisplayAllActiveStates();
	}
}

