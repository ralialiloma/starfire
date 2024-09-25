// Fill out your copyright notice in the Description page of Project Settings.


#include "StateCallstack.h"

#include "SF_CharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/DebugSubsystem.h"
#include "VisualLogger/VisualLoggerTypes.h"


DEFINE_LOG_CATEGORY_STATIC(SF_StateCallStack, Log, All);

bool UStateCallstack::TryAddState(TSubclassOf<UBaseState> BaseStateClass)
{
	//Check if state is valid
	if (!IsValid(BaseStateClass)||!UKismetSystemLibrary::IsValidClass(BaseStateClass))
	{
		UE_LOG(SF_StateCallStack, Error, TEXT("Invalid BaseStateClass"))
		return false;
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
	UE_LOG(SF_StateCallStack, Error, TEXT("Message %i"),CreatedState->GetPriority());
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

	if (!IsValid(CreatedState))
	{
		UE_LOG(SF_StateCallStack, Error, TEXT("Created state is invalid"))
		return false;
	}
	
	RunActiveStateFeatures(CreatedState,Enter, Data);
	
	UE_LOG(SF_StateCallStack, Log, TEXT("Added New State %s"),*BaseStateClass->GetName())

	return true;
}

bool UStateCallstack::TryRemoveState(TSubclassOf<UBaseState> BaseStateClass)
{
	const FStateModuleDataStruct& Data = FStateModuleDataStruct();

	if (!IsValid(BaseStateClass))
    {
    	UE_LOG(SF_StateCallStack, Error, TEXT("StateClass To delete is invalid"));
    	return false;
    }

	UBaseState* FoundState = GetStateByClass(BaseStateClass);

	if (!IsValid(FoundState))
	{
		return false;
	}
	
	RunActiveStateFeatures(FoundState,Exit, Data);
	for (UBaseState* State: ActiveStatesByPriority)
	{
		if (State->IsA(BaseStateClass))
		{
			ActiveStatesByPriority.Remove(State);
			UE_LOG(SF_StateCallStack, Log, TEXT("Removed State %s"),*BaseStateClass->GetName())
			return true;
		}
	}
	return false;
}

bool UStateCallstack::ToggleStateByBool(const TSubclassOf<UBaseState> BaseStateClass, const bool bValue)
{

	if (!IsValid(BaseStateClass))
	{
		UE_LOG(
			SF_StateCallStack,
			Error ,
			TEXT("Invalid Base State Class To Toggle State By Bool"))
		return false;
	}
	
	if (bValue)
	{
		return  TryAddState(BaseStateClass);
	}
	else
	{
		return  TryRemoveState(BaseStateClass);
	}
}


void UStateCallstack::RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput, FStateModuleDataStruct Data)
{
	if (UBaseStateFeature* FoundFeature = GetActiveFeature(FeatureClassToRun))
	{
		if (!IsValid(FoundFeature))
		{
			UE_LOG(
				SF_StateCallStack,
				Warning ,
				TEXT("Found Features is invalid"))
		}
		
		FoundFeature->RunAction(CallInput, Data);
		return;
	}

	if (IsValid(FeatureClassToRun))
	{
		UE_LOG(
			SF_StateCallStack,
			Warning ,
			TEXT("Cannot RunCallStack, Feature: %s not found"),
			*FeatureClassToRun.Get()->GetName())
	}
	else
	{
		UE_LOG(
			SF_StateCallStack,
			Warning ,
			TEXT("Cannot RunCallStack, Feature: Invalid Feature not found"))
	}
}


void UStateCallstack::SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove)
{
	//Remove Old State
	TryRemoveState(StateToRemove);

	//Add New State
	TryAddState(StateToAdd);
}

TArray<UBaseState*> UStateCallstack::GetAllActiveStates() const
{
	return ActiveStatesByPriority;
}

TArray<TSubclassOf<UBaseStateFeature>> UStateCallstack::GetAllFeatures()
{
	TArray<TSubclassOf<UBaseStateFeature>> BaseStateFeatures{};
	for (UBaseState* State: ActiveStatesByPriority)
	{
		BaseStateFeatures.Append(State->GetAllOwnedFeatures());	
	}
	return BaseStateFeatures ;
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
	return nullptr;
}


void UStateCallstack::RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput, const FStateModuleDataStruct& Data)
{
	if (!IsValid(StateToRunOn))
	{
		UE_LOG(
			SF_StateCallStack,
			Error ,
			TEXT("State To Run Active Feature On Is Invalid"));
		return;
	}
	
	TArray<TSubclassOf<UBaseStateFeature>> OwnedFeatures = StateToRunOn->GetAllOwnedFeatures();
	
	//Run Callstack for specific class
	for (TSubclassOf<UBaseStateFeature> OwnedFeature:OwnedFeatures)
	{
		UBaseStateFeature* ActiveFeature = GetActiveFeature(OwnedFeature);
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




