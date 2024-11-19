

#include "SF_EQS_Scheduler.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "Starfire/Utility/AsyncUtility.h"


void USf_EQS_Scheduler::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()->void{Tick(0);},1.f,true);

	//Register Ticker
	//TickHandle = FTSTicker::GetCoreTicker().AddTicker(
	//FTickerDelegate::CreateUObject(this, &USf_EQS_Scheduler::Tick));
}

void USf_EQS_Scheduler::Deinitialize()
{
	//FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
	Super::Deinitialize();
}

FGuid USf_EQS_Scheduler::ScheduleRequest(const FScheduledEnvRequest ScheduledEnvRequest)
{
	if (!ScheduledEnvRequest.QueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScheduleRequest: Invalid request provided."));
	}
	
	ScheduledRequests.Add(ScheduledEnvRequest);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::FromInt(ScheduledRequests.Num()));

	return ScheduledEnvRequest.UniqueIdentifier;
}

void USf_EQS_Scheduler::RunRequest(const FScheduledEnvRequest& Request)
{
	if (!Request.QueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunRequest: Invalid request."));
		return;
	}

	UEnvQueryManager* EQSManager = UEnvQueryManager::GetCurrent(GetWorld());
	if (!EQSManager)
	{
		UE_LOG(LogTemp, Error, TEXT("RunRequest: Could not find EQS Manager."));
		return;
	}

	// Run the EQS query
	 UEnvQueryInstanceBlueprintWrapper* Wrapper =
	 	EQSManager->RunEQSQuery(GetWorld(), Request.QueryTemplate, Request.Querier, Request.RunMode, nullptr);
	ActiveRequests.Add(Wrapper,Request);

	Wrapper->GetOnQueryFinishedEvent().AddDynamic(this,&USf_EQS_Scheduler::OnQueryFinished);
}

bool USf_EQS_Scheduler::Tick(float DeltaTime)
{
	if (ScheduledRequests.Num() <= 0||ActiveRequests.Num()>0)
		return true;
	
	// Get the next request to process
	FScheduledEnvRequest Request = ScheduledRequests[0];
	ScheduledRequests.RemoveAt(0);
	RunRequest(Request);
	return true;
}

void USf_EQS_Scheduler::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(ScheduledRequests.Num()));
	
	const FScheduledEnvRequest* RequestPtr =  ActiveRequests.Find(QueryInstance);

	if (RequestPtr==nullptr)
		return;

	ActiveRequests.Remove(QueryInstance);
	FScheduledEnvRequest Request = *RequestPtr;

	TArray<FVector> Locations{};
	QueryInstance->GetQueryResultsAsLocations(Locations);

	OnEQSResult.Broadcast(Locations,Request.UniqueIdentifier);
	
}
