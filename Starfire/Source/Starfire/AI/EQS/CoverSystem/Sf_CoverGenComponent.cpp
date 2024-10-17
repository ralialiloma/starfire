
#include "Sf_CoverGenComponent.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/DebugSubsystem.h"


USf_CoverGenComponent::USf_CoverGenComponent(): CoverEnvQuery(nullptr), CoverQueryInstance(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USf_CoverGenComponent::BeginPlay()
{
	Super::BeginPlay();
	StartRunQuery();
}

void USf_CoverGenComponent::GetCoverLocations(TArray<FVector>& CoverLocations) const
{
	CoverLocations.Empty();
	if(!IsValid(CoverQueryInstance))
		return;
	
	TArray<FVector> QueryLocations{};
	GetQueryLocations(QueryLocations);
	for (int i= 0; i<QueryLocations.Num();i++)
	{
		if (CoverQueryInstance->GetItemScore(i)>0)
		{
			CoverLocations.Add(QueryLocations[i]);
		}
	}
}

void USf_CoverGenComponent::GetPeakLocations(TArray<FVector>& PeakLocations) const
{
	PeakLocations.Empty();
	if(!IsValid(CoverQueryInstance))
		return;
	
	TArray<FVector> QueryLocations{};
	GetQueryLocations(QueryLocations);

	for (int i= 0; i<QueryLocations.Num();i++)
	{
		if (CoverQueryInstance->GetItemScore(i)<0.2f)
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

void USf_CoverGenComponent::DebugCoverLocations() const
{
	if (!IsValid(CoverQueryInstance))
		return;

	TArray<FVector> CoverLocations{};
	GetCoverLocations(CoverLocations);
	for (FVector Location: CoverLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this,Location,40,6,FColor::Purple,QueryUpdateRate,1);
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
	CoverQueryInstance = QueryInstance;
	
	if (UDebugSubsystem::GetAIDebug(EDebugType::Visual))
	{
		DebugCoverLocations();
	}
}




