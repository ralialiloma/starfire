// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_ReservedCoverLocs.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"



UEnvQueryTest__ReservedCoverLocs::UEnvQueryTest__ReservedCoverLocs(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	ScoringEquation = EEnvTestScoreEquation::Constant;
}

void UEnvQueryTest__ReservedCoverLocs::RunTest(FEnvQueryInstance& QueryInstance) const
{
	
	
}

FText UEnvQueryTest__ReservedCoverLocs::GetDescriptionTitle() const
{
	return FText::FromString("Close To Cover Wall");
}

FText UEnvQueryTest__ReservedCoverLocs::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}

