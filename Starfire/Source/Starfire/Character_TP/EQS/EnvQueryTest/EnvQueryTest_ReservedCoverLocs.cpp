// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_ReservedCoverLocs.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"

UEnvQueryTest_ReservedCoverLocs::UEnvQueryTest_ReservedCoverLocs(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_ReservedCoverLocs::RunTest(FEnvQueryInstance& QueryInstance) const
{

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		//Check Distance To Next Cover
		const FVector CoverLocation = GetItemLocation(QueryInstance,It.GetIndex());
		const bool CoverValid =  InReservedCover(CoverLocation);
		It.SetScore(TestPurpose, FilterType,CoverValid,CoverShouldBeValid);
	}
	
}

FText UEnvQueryTest_ReservedCoverLocs::GetDescriptionTitle() const
{
	return FText::FromString("Reserved Cover Locations");
}

FText UEnvQueryTest_ReservedCoverLocs::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}

//Private Functions
bool UEnvQueryTest_ReservedCoverLocs::InReservedCover(const FVector CoverLocation) const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance==nullptr)
		return true;
	
	const auto CoverGen =  GameInstance->GetSubsystem<UNavigationTargetSubsystem>();

	if (CoverGen==nullptr)
		return true;

	return  CoverGen->LocationInReservedCover(CoverLocation,RadiusToCheck);
}







