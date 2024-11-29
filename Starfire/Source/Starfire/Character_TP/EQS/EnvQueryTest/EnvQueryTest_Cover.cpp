// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_Cover.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointSubsystem.h"

UEnvQueryTest_Cover::UEnvQueryTest_Cover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	ScoringEquation = EEnvTestScoreEquation::Constant;
}

void UEnvQueryTest_Cover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	if (!GetWorld())
		return;

	if (!GetWorld()->GetGameInstance())
		return;
	
	for (FEnvQueryInstance::ItemIterator Iterator(this, QueryInstance); Iterator; ++Iterator)
	{
		ASf_TetherPointGen* TetherPointGen =  GetWorld()->GetGameInstance()->GetSubsystem<USf_TetherPointSubsystem>()->GetTetherPointGen();
		TArray<UTetherPoint*> TetherPoints =  TetherPointGen->GetRelevantTetherPoints();

		bool bFoundTetherPoint = false;
		for (UTetherPoint* Point: TetherPoints)
		{
			const FVector ItemLocattion = QueryInstance.GetItemAsLocation(Iterator.GetIndex());
			if (FVector::Distance(Point->CenterLocation,ItemLocattion)<=RadiusToCheck)
			{
				Iterator.SetScore(TestPurpose, EEnvTestFilterType::Type::Range,Point->CoverPotential,MinCoverPotential, MaxCoverPotential);
				bFoundTetherPoint = true;
				break;
			}
		}
		if (!bFoundTetherPoint)
		{
			Iterator.SetScore(TestPurpose, EEnvTestFilterType::Type::Range,-MinCoverPotential,MinCoverPotential, MaxCoverPotential);
		}
		
	}
	
}

FText UEnvQueryTest_Cover::GetDescriptionTitle() const
{
	return FText::FromString("Test For Cover");
}

FText UEnvQueryTest_Cover::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}









