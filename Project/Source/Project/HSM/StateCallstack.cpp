// Fill out your copyright notice in the Description page of Project Settings.


#include "StateCallstack.h"

#include "SF_CharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VisualLogger/VisualLoggerTypes.h"

DEFINE_LOG_CATEGORY_STATIC(SF_StateCallStack, Display, Display);

bool UStateCallstack::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
	//Check if state is valid
	if (BaseStateClass == nullptr)
	{
		UE_LOG(SF_StateCallStack, Error, TEXT("Invalid BaseStateClass"))
		return  false;
	}
		
	
	//Check if state already exits
	for (UBaseState* State: ActiveStatesByPriority)
	{
		if (State->GetClass() == BaseStateClass)
			return  false;
	}

	//Add State
	TArray<UBaseState*> UnsortedStates = ActiveStatesByPriority;
	
	UBaseState* CreatedState = NewObject<UBaseState>(this,BaseStateClass);
	CreatedState->StateCallstack = this;
	CreatedState->CreateFeatures(StateDefintions);
	UnsortedStates.Add(CreatedState);
	//Sort by priority
	UnsortedStates.Sort
	(
		[](const UBaseState& StateA, const UBaseState& StateB)
		{
			return StateA.GetPriority() > StateB.GetPriority();
		}
	);
	ActiveStatesByPriority = UnsortedStates;
	FStateModuleDataStruct Data = FStateModuleDataStruct();
	RunActiveStateFeatures(GetStateByClass(BaseStateClass),Enter, Data);
	return  true;
}

bool UStateCallstack::TryRemoveState(TSubclassOf<UBaseState> BaseStateClass)
{
	const FStateModuleDataStruct& Data = FStateModuleDataStruct();
	RunActiveStateFeatures(GetStateByClass(BaseStateClass),Exit, Data);
	for (UBaseState* State: ActiveStatesByPriority)
	{
		if (State->IsA(BaseStateClass))
		{
			ActiveStatesByPriority.Remove(State);
			return true;
		}
	}
	return false;
}


void UStateCallstack::RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput, FStateModuleDataStruct Data)
{
	if (UBaseStateFeature* FoundFeature = GetActiveFeature(FeatureClassToRun))
	{
		FoundFeature->RunAction(CallInput, Data);
	}
}


void UStateCallstack::SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove)
{
	//Remove Old State
	TryRemoveState(StateToRemove);

	//Add New State
	TryAddState(StateToAdd);
}

TArray<UBaseState*> UStateCallstack::GetActiveStates()
{
	return ActiveStatesByPriority;
}

UBaseStateFeature* UStateCallstack::GetActiveFeature(TSubclassOf<UBaseStateFeature> FeatureClassToRun)
{
	UBaseStateFeature* FoundFeature = nullptr;
	
	for (int i = 0; i<ActiveStatesByPriority.Num();i++)
	{
		if (ActiveStatesByPriority[i]->TryGetFeatureFast(FeatureClassToRun,FoundFeature))
		{
			return FoundFeature;
		}
	}
	return  nullptr;
}


void UStateCallstack::RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput, const FStateModuleDataStruct& Data)
{
	TArray<TSubclassOf<UBaseStateFeature>> OwnedFeatures = StateToRunOn->GetAllOwnedFeatures();
	
	//Run Callstack for specific class
	UBaseStateFeature* ActiveFeature;
	for (TSubclassOf<UBaseStateFeature> OwnedFeature:OwnedFeatures)
	{
		ActiveFeature = GetActiveFeature(OwnedFeature);
		if (StateToRunOn->ContainsThisFeature(ActiveFeature))
		{
			ActiveFeature->RunAction(CallInput, Data);
		}
	}
	
}

UBaseState* UStateCallstack::GetStateByClass(TSubclassOf<UBaseState> ClassToSearchFor)
{
	for (UBaseState* ActiveState:ActiveStatesByPriority)
	{
		if (ActiveState->GetClass() == ClassToSearchFor)
			return  ActiveState;
	};
	return nullptr;
}




