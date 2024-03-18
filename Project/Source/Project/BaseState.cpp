// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseState.h"

#include "StateCallstack.h"


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

void UBaseState::CreateFeatures()
{
	for (TSubclassOf<UBaseStateFeature> FeatureClass:OwnedFeatures)
	{
		UBaseStateFeature* ObjectToCreate = NewObject<UBaseStateFeature>(this, FeatureClass);
		ObjectToCreate->Initialize(StateCallstack);
		Features.Add(ObjectToCreate);;
	}
}

void UBaseState::PostInitProperties()
{
	Super::PostInitProperties();
	//CreateFeatures();
}

TArray<TSubclassOf<UBaseStateFeature>> UBaseState::GetAllOwnedFeatures()
{
	return  OwnedFeatures;
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






