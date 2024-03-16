// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseState.h"


int UBaseState::GetPriority()
{
	return  Priority;
}

template<std::derived_from<UBaseStateFeature>FeatureClass>
bool UBaseState::TryGetFeature(FeatureClass*& Feature)
{
	for(UBaseStateFeature* FeatureToCheck: BaseStateFeatures)
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
	for(UBaseStateFeature* FeatureToCheck: BaseStateFeatures)
	{
		if(FeatureToCheck->IsA(FeatureClass))
		{
			Feature = FeatureToCheck;
			return  true;
		}
	}

	Feature = nullptr;
	return false;
}
