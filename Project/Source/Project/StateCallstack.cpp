// Fill out your copyright notice in the Description page of Project Settings.


#include "StateCallstack.h"

#include "CharacterStateMachine.h"

bool UStateCallstack::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
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

void UStateCallstack::RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput callInput)
{
	for (int i = 0; i<ActiveStatesByPriority.Num();i++)
	{
		UBaseStateFeature* FoundFeature = nullptr;
		if (ActiveStatesByPriority[i]->TryGetFeatureFast(FeatureClassToRun,FoundFeature))
		{
			switch (callInput)
			{
				case Enter:
					FoundFeature->EnterState();
				break;
				case Run:
					FoundFeature->RunFeature();
				break;
				case Exit:
					FoundFeature->ExitState();
				break;
			}
			return;
		}
	}
}





