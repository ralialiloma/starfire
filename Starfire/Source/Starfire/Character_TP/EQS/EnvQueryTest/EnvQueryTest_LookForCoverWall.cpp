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
	
	for (FEnvQueryInstance::ItemIterator Iterator(this, QueryInstance); Iterator; ++Iterator)
	{
		//Check Distance To Next Cover
		const FVector Location = GetItemLocation(QueryInstance,Iterator.GetIndex());
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
			FLinearColor::Red,
			1.0f);

		float DistanceToCover = HitResult.Distance;
		float Score = 0;

		if (HitResult.bBlockingHit)
		{
			Score = (DistanceToCover - MinDistanceToCover) / MaxDistanceToCover;
			Score = 1 - FMath::Min(1.0f, Score);
			Score = FMath::Clamp(Score,0.1f,1);
		}

		//FColor DebugColor = FColor(Score*255,0,0);
		//UKismetSystemLibrary::DrawDebugPoint(this,Location,25,DebugColor,1.0f);
		
		Iterator.SetScore(EEnvTestPurpose::Type::Score, EEnvTestFilterType::Type::Range,Score,0, 1);
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
