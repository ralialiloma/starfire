// Fill out your copyright notice in the Description page of Project Settings.


#include "StateCallstack.h"

#include "CharacterStateMachine.h"

bool UStateCallstack::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
	//Check if state is valid
	if (BaseStateClass == nullptr)
		return  false;
	
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
	UnsortedStates.Add(CreatedState);
	//Sort by priority
	UnsortedStates.Sort
	(
		[](UBaseState& StateA, UBaseState& StateB)
		{
			return StateA.GetPriority()<StateB.GetPriority();
		}
	);
	ActiveStatesByPriority = UnsortedStates;
	return  true;
}

bool UStateCallstack::TryRemoveState(TSubclassOf<UBaseState> BaseStateClass)
{
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


void UStateCallstack::RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput,FStateModuleDataStruct Data)
{
	UBaseStateFeature* FoundFeature = GetActiveFeature(FeatureClassToRun);
	
	
	if (FoundFeature)
	{
		FoundFeature->RunAction(CallInput, Data);
	}
	
}


void UStateCallstack::SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove, FStateModuleDataStruct Data)
{
	//Remove Old State
	RunActiveStateFeatures(GetStateByClass(StateToRemove),Exit,Data);
	TryRemoveState(StateToRemove);

	//Add New State
	TryAddState(StateToAdd);
	RunActiveStateFeatures(GetStateByClass(StateToAdd),Enter, Data);
	
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


void UStateCallstack::RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput, FStateModuleDataStruct Data)
{
	TArray<TSubclassOf<UBaseStateFeature>> OwnedFeatures = StateToRunOn->GetAllOwnedFeatures();
	
	//Run Callstack for specific class
	UBaseStateFeature* ActiveFeature;
	for (TSubclassOf<UBaseStateFeature> OwnedFeature:OwnedFeatures)
	{
		ActiveFeature = GetActiveFeature(OwnedFeature);
		if (StateToRunOn->ContainsThisFeature(ActiveFeature))
		{
			ActiveFeature->RunAction(CallInput,Data);
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




