// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_LookForCoverWall.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/KismetSystemLibrary.h"

UEnvQueryTest_LookForCoverWall::UEnvQueryTest_LookForCoverWall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HideFrom = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	ScoringEquation = EEnvTestScoreEquation::Constant;
}

void UEnvQueryTest_LookForCoverWall::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();
	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	//Check If Hide From Context is valid 
	TArray<FVector>  HideFromCtx;
	if (!QueryInstance.PrepareContext(HideFrom, HideFromCtx))
	{
		return;
	}
	FVector LocationToHideFrom = HideFromCtx[0];

	//Check if ignored Ctx is valid
	TArray<AActor*>  IgnoreCtx;
	if (!QueryInstance.PrepareContext(Ignore, IgnoreCtx))
	{
		return;
	}

	//Ignored Actors
	TArray<AActor*> IgnoredActors;
	QueryInstance.PrepareContext(UEnvQueryContext_Querier::StaticClass(), IgnoredActors);
	IgnoredActors.Append(IgnoreCtx);
	

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		//Check Distance To Next Cover
		const FVector Location = GetItemLocation(QueryInstance,It.GetIndex());
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(
			this,
			Location,
			LocationToHideFrom,
			TraceTypeQuery,
			false,
			IgnoredActors,
			DebugDrawTraceType,
			HitResult,
			true,
			FLinearColor::Blue,
			FLinearColor::Red
			);
		float Distance = MinThresholdValue-1;
        Distance = HitResult.Distance;
		float Score = Distance;

		if (!HitResult.bBlockingHit|| Distance<MinThresholdValue||Distance>MaxThresholdValue)
		{
			Score = 0;
		}

		It.SetScore(TestPurpose, FilterType,Score,MinThresholdValue, MaxThresholdValue);
		
	}
	
}

FText UEnvQueryTest_LookForCoverWall::GetDescriptionTitle() const
{
	return FText::FromString("Close To Cover Wall");
}

FText UEnvQueryTest_LookForCoverWall::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
