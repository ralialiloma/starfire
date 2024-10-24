// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseState.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "StateCallstack.h"
#include "Kismet/KismetSystemLibrary.h"


int UBaseState::GetPriority() const
{
	return  Priority;
}

template<std::derived_from<UBaseStateFeature>FeatureClass>
bool UBaseState::TryGetFeature(FeatureClass*& Feature)
{
	for(UBaseStateFeature* FeatureToCheck: Features)
	{
		if(FeatureToCheck->IsA(FeatureClass::StaticClass))
		{
			Feature = Cast<FeatureClass>(FeatureToCheck);
			return  true;
		}
	}

	Feature = nullptr;
	return false;
}

bool UBaseState::TryGetFeatureFast(TSubclassOf<UBaseStateFeature> FeatureClass,UBaseStateFeature*& Feature)
{
	for(UBaseStateFeature* FeatureToCheck: Features)
	{
		if(FeatureToCheck && FeatureToCheck->IsA(FeatureClass))
		{
			Feature = FeatureToCheck;
			return  true;
		}
	}
	Feature = nullptr;
	return false;
}

void UBaseState::CreateFeatures(const FSoftObjectPath& StateDefintionDT)
{
	ImportStateDefinition(StateDefintionDT);
	
	//Create Features
	for (TSubclassOf<UBaseStateFeature> FeatureClass:OwnedFeatures)
	{
		if (!IsValid(FeatureClass)||!IsValid(FeatureClass.Get()))
		{
			UE_LOG(SF_BaseState,Warning,TEXT("Invalid Feature Class"));
			continue;
		}
		
		UBaseStateFeature* ObjectToCreate = NewObject<UBaseStateFeature>(this, FeatureClass);
		ObjectToCreate->Initialize(StateCallstack);
		Features.Add(ObjectToCreate);;
	}
}

void UBaseState::ImportStateDefinition(const FSoftObjectPath& StateDefintionDT)
{
	TArray<FStateDefinition> StateDefinitions =
		USf_FunctionLibrary::GetRowDataFromDT<FStateDefinition>(StateDefintionDT);
	
	for (const FStateDefinition Definition:StateDefinitions)
	{
		TSubclassOf<UBaseState> RowState = Definition.State;

		if (!IsValid(RowState) || !UKismetSystemLibrary::IsValidClass(RowState))
			continue;

		if (!RowState->IsChildOf(GetClass()))
			continue;

		TArray<TSubclassOf<UBaseStateFeature>> FeaturesFromDT = Definition.OwnedFeatures.Array();
		TArray<TSubclassOf<UBaseStateFeature>> FeaturesToImport{};
		for (TSubclassOf<UBaseStateFeature> Feature: FeaturesFromDT)
		{
			if (!IsValid(Feature)||!IsValid(Feature.Get()))
			{
				UE_LOG(
					SF_BaseState
					,Warning,
					TEXT("Found Invalid Feature Class for"));
				continue;
			}

			FeaturesToImport.Add(Feature);
		}
		
		OwnedFeatures = FeaturesToImport;
		Priority = Definition.Priority;
		break;
	}
}


FStateDefinition::FStateDefinition(): Priority(0)
{
	State = nullptr;
	OwnedFeatures = TSet<TSubclassOf<UBaseStateFeature>> {};
	Priority = 0;
}

void UBaseState::PostInitProperties()
{
	Super::PostInitProperties();
	//CreateFeatures();
}

TArray<TSubclassOf<UBaseStateFeature>> UBaseState::GetAllOwnedFeatures()
{
	return OwnedFeatures;
}

bool UBaseState::ContainsThisFeature(UBaseStateFeature* Feature)
{
	for(UBaseStateFeature* ExistingFeature:Features)
	{
		if (ExistingFeature == Feature)
		{
			return true;
		}
	}
	return false;
}






