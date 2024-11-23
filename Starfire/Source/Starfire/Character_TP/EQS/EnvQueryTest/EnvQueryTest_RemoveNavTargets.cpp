// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_RemoveNavTargets.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"

UEnvQueryTest_RemoveNavTargets::UEnvQueryTest_RemoveNavTargets(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_RemoveNavTargets::RunTest(FEnvQueryInstance& QueryInstance) const
{
	for (int32 Index = QueryInstance.Items.Num() - 1; Index >= 0; --Index)
	{
		// Get the location of the item
		const FVector CoverLocation = GetItemLocation(QueryInstance, Index);
		
		if (HasCloseNavTarget(CoverLocation))
		{
			QueryInstance.Items.RemoveAtSwap(Index);
		}
	}
	/*for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		//Check Distance To Next Cover
		const FVector CoverLocation = GetItemLocation(QueryInstance,It.GetIndex());
		const bool CoverValid =  HasCloseNavTarget(CoverLocation);
		It.SetScore(TestPurpose, FilterType,CoverValid,CoverShouldBeValid);
	}*/
	
}

FText UEnvQueryTest_RemoveNavTargets::GetDescriptionTitle() const
{
	return FText::FromString("Reserved Cover Locations");
}

FText UEnvQueryTest_RemoveNavTargets::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}

//Private Functions
bool UEnvQueryTest_RemoveNavTargets::HasCloseNavTarget(const FVector CoverLocation) const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance==nullptr)
		return true;
	
	const auto CoverGen =  GameInstance->GetSubsystem<UNavigationTargetSubsystem>();

	if (CoverGen==nullptr)
		return false;

	return  CoverGen->HasCloseNavTarget(CoverLocation,RadiusToCheck);
}







