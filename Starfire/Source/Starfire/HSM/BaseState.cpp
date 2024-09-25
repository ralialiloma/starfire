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

void UBaseState::CreateFeatures(const FSoftObjectPath SoftObjectPath)
{
	//Find Owned Features
	UDataTable* DataTable = Cast<UDataTable>(SoftObjectPath.ResolveObject());
	
	if (!DataTable)
	{
		DataTable = Cast<UDataTable>(SoftObjectPath.TryLoad());
		UE_LOG(SF_BaseState,Warning,TEXT("Could not find DT for states"))
		return;
	}

	TObjectPtr<UScriptStruct> RowStruct = DataTable->RowStruct;

	if (!IsValid(RowStruct))
	{
		UE_LOG(SF_BaseState,Warning,TEXT("Invalid Row Struct in DT"))
		return;
	}
	
	if (RowStruct != FStateDefinition::StaticStruct())
	{
		UE_LOG(
			SF_BaseState,
			Warning,
			TEXT("Wrong DT Row Struct. Row Struct needs to be of type %s but is of type %s instead "),
			*FStateDefinition::StaticStruct()->GetName(),*DataTable->RowStruct.GetClass()->GetName());
		return;
	}
	
	const TArray<FName> AllRowNames = DataTable->GetRowNames();
	for (const FName RowName:AllRowNames)
	{
		const FStateDefinition* Row = DataTable->FindRow<FStateDefinition>(RowName,"");

		if (Row==nullptr)
		{
			UE_LOG(
			SF_BaseState,
			Warning,
			TEXT("Found Invalid Row"));
			continue;
		}

		TSubclassOf<UBaseState> RowState = Row->State;
		if (!IsValid(RowState)||!IsValid( RowState.Get()))
		{
			UE_LOG(SF_BaseState,Warning,TEXT("Invalid State for %s"),*RowName.ToString())
			continue;
		}

		if (!RowState->IsChildOf(GetClass()))
			continue;

		TArray<TSubclassOf<UBaseStateFeature>> FeaturesFromDT = Row->OwnedFeatures.Array();
		TArray<TSubclassOf<UBaseStateFeature>> FeaturesToImport{};
		for (TSubclassOf<UBaseStateFeature> Feature: FeaturesFromDT)
		{
			if (!IsValid(Feature)||!IsValid(Feature.Get()))
			{
				UE_LOG(
					SF_BaseState
					,Warning,
					TEXT("Found Invalid Feature Class for %s"),*RowName.ToString());
				continue;
			}

			FeaturesToImport.Add(Feature);
		}
		
		OwnedFeatures = FeaturesToImport;
		Priority = Row->Priority;
		break;
	}

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






