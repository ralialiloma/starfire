// Fill out your copyright notice in the Description page of Project Settings.


#include "StateCallstack.h"

#include "CharacterStateMachine.h"

bool UStateCallstack::TryAddActiveStates(TSubclassOf<UBaseState> BaseStateClass)
{
	//Check if state already exits
	for (UBaseState* State: ActiveStatesByPriority)
	{
		if (State->GetClass() == BaseStateClass)
			return  false;
	}

	//Add State
	TArray<UBaseState*> UnsortedStates = ActiveStatesByPriority;
	UnsortedStates.Add(NewObject<UBaseState>(this,BaseStateClass));
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





