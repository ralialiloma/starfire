
#include "Sf_CoverGenComponent.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


USf_CoverGenComponent::USf_CoverGenComponent(): CoverEnvQuery(nullptr), CoverQueryInstance(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USf_CoverGenComponent::BeginPlay()
{
	Super::BeginPlay();
	StartRunQuery();
}

TArray<FVector> USf_CoverGenComponent::GetCoverLocations() const
{
	return SavedCoverLocations;
}

TArray<FVector> USf_CoverGenComponent::GetPeakLocations() const
{
	return SavedPeakLocations;;
}

void USf_CoverGenComponent::UpdateCoverAndPeakLocations(const float MinValue)
{
	SavedCoverLocations.Empty();
	if(!IsValid(CoverQueryInstance))
		return;

	const FEnvQueryResult* QueryResult = GetQueryResult();
	if (!QueryResult)
		return;
	
	const int ResultCount = QueryResult->Items.Num();
	for (int i = 0; i < ResultCount; i++)
	{
		float ItemScore = QueryResult->GetItemScore(i);
		if (ItemScore>FMath::Max(0,MinValue))
			SavedCoverLocations.Add(QueryResult->GetItemAsLocation(i));
		else if (FMath::IsNearlyEqual(ItemScore, 0.0f))
			SavedPeakLocations.Add(QueryResult->GetItemAsLocation(i));
	}

}

void USf_CoverGenComponent::FindPeakLocations(TArray<FVector>& PeakLocations) const
{
	PeakLocations.Empty();
	if(!IsValid(CoverQueryInstance))
		return;
	
	TArray<FVector> QueryLocations{};
	GetQueryLocations(QueryLocations);

	for (int i= 0; i<QueryLocations.Num();i++)
	{
		if (CoverQueryInstance->GetItemScore(i)==0)
		{
			PeakLocations.Add(QueryLocations[i]);
		}
	}
}

void USf_CoverGenComponent::StartRunQuery()
{
	TWeakObjectPtr<USf_CoverGenComponent> WeakSelf = this;
	FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[WeakSelf]()->void
	{
		FAsyncUtility::WaitForSeconds(WeakSelf->QueryUpdateRate,WeakSelf,0.1);
		while (true)
		{
			FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf]()->void
			{
				if (!WeakSelf.IsValid())
					return;

				WeakSelf->DoRunQuery();
			});

			if (!WeakSelf.IsValid())
            	return;
			
			FAsyncUtility::WaitForSeconds(WeakSelf->QueryUpdateRate,WeakSelf,0.1);
		}
	});
}

void USf_CoverGenComponent::GetQueryLocations(TArray<FVector>& QueryLocations) const
{
	QueryLocations.Empty();
	
	if (!IsValid(CoverQueryInstance))
		return;

	CoverQueryInstance->GetQueryResultsAsLocations(QueryLocations);
}

const FEnvQueryResult* USf_CoverGenComponent::GetQueryResult() const
{
	if (!IsValid(CoverQueryInstance))
		return nullptr;

	return CoverQueryInstance->GetQueryResult();
}

void USf_CoverGenComponent::DebugCoverLocations() const
{
	if (!UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::EQS::CoverLocation,EDebugType::Visual))
		return;
	
	if (!IsValid(CoverQueryInstance))
		return;

	TArray<FVector> CoverLocations = GetCoverLocations();;

	for (FVector Location: CoverLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Location,40,6,FColor::Purple,QueryUpdateRate,1);
	}
}

void USf_CoverGenComponent::DebugPeakLocations() const
{
	if (!UDebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::TP::EQS::PeakLocation,EDebugType::Visual))
		return;
	
	if (!IsValid(CoverQueryInstance))
		return;

	TArray<FVector> CoverLocations = GetPeakLocations();;

	for (FVector Location: CoverLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Location,40,6,FColor::Green,QueryUpdateRate,1);
	}
}

void USf_CoverGenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USf_CoverGenComponent::DoRunQuery()
{
	if (!CoverEnvQuery)
	{
		UE_LOG(LogTemp, Warning, TEXT("CoverEnvQuery is null"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this,
		CoverEnvQuery,
		GetOwner(),
		EEnvQueryRunMode::AllMatching,
		UEnvQueryInstanceBlueprintWrapper::StaticClass()
	);

	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &USf_CoverGenComponent::OnQueryFinished);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Query Instance"));
	}
}

void USf_CoverGenComponent::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
		return;
	
	CoverQueryInstance = QueryInstance;
	UpdateCoverAndPeakLocations(0.9f);
	FindPeakLocations(SavedPeakLocations);
	
	DebugCoverLocations();
	DebugPeakLocations();
	
}




