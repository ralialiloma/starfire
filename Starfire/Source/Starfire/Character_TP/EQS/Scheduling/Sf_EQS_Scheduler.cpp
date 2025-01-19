

#include "Sf_EQS_Scheduler.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


FScheduledEnvRequest::FScheduledEnvRequest(UObject* InQuerier, UEnvQuery* InQueryTemplate, TEnumAsByte<EEnvQueryRunMode::Type> InRunMode):
	Querier(InQuerier),
	QueryTemplate(InQueryTemplate),
	RunMode(InRunMode),
	UniqueIdentifier(FGuid::NewGuid())
{
}

FScheduledEnvRequest::FScheduledEnvRequest(): RunMode()
{
	UniqueIdentifier = FGuid::NewGuid();
	Querier = nullptr;
	QueryTemplate = nullptr;
	RunMode = EEnvQueryRunMode::SingleResult;
	UniqueIdentifier = FGuid::NewGuid();
}

void USf_EQS_Scheduler::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()->void{Tick();},Tickrate,true);
	UEnvQueryManager::GetCurrent(GetWorld())->SetAllowTimeSlicing(true);
}

void USf_EQS_Scheduler::Deinitialize()
{
	//FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Super::Deinitialize();
}

USf_EQS_Scheduler* USf_EQS_Scheduler::GetCurrent(const UWorld* World)
{
	if (IsValid(World))
		return  World->GetSubsystem<USf_EQS_Scheduler>();
	return 	nullptr;
}

FGuid USf_EQS_Scheduler::ScheduleRequest(const FScheduledEnvRequest ScheduledEnvRequest)
{
	if (!ScheduledEnvRequest.QueryTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScheduleRequest: Invalid request provided."));
	}

	FScheduledEnvRequest FoundSimilarRequest;
	if (HasExistingSimilarRequest(ScheduledEnvRequest, FoundSimilarRequest))
	{
		return FoundSimilarRequest.UniqueIdentifier;
	}
	
	ScheduledRequests.Add(ScheduledEnvRequest);
	FString DebugString = FString::Printf(TEXT("Currently have %i requests scheduled"),ScheduledRequests.Num()); 
	SF_LOG(LogTemp,Type::Warning,DebugString,TP::EQS::Scheduler);
	return ScheduledEnvRequest.UniqueIdentifier;
}

bool USf_EQS_Scheduler::HasExistingSimilarRequest(FScheduledEnvRequest RequestToCompare, FScheduledEnvRequest OutFoundSimilarRequest)
{
	const FScheduledEnvRequest* FoundRequest =  ScheduledRequests.FindByPredicate([RequestToCompare](const FScheduledEnvRequest& Request)->bool
		{
			return RequestToCompare.Querier == Request.Querier && RequestToCompare.QueryTemplate == Request.QueryTemplate;
		});
	return FoundRequest != nullptr;
}

void USf_EQS_Scheduler::RunRequest(const FScheduledEnvRequest& Request)
{
	if (!Request.Querier)
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this, "Invalid Querier");
		return;
	}
	
	if (!Request.QueryTemplate)
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this, "Invalid Query Template");
		return;
	}

	UEnvQueryManager* EQSManager = UEnvQueryManager::GetCurrent(GetWorld());
	if (!EQSManager)
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this, "Could not find EQS Manager.");
		return;
	}

	// Run the EQS query
	 UEnvQueryInstanceBlueprintWrapper* Wrapper =
	 	EQSManager->RunEQSQuery(GetWorld(), Request.QueryTemplate, Request.Querier, Request.RunMode, nullptr);
	ActiveRequests.Add(Wrapper,Request);

	if (!IsValid(Wrapper))
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this,"Invalid QueryInstance Wrapper");
		return;
	}

	Wrapper->GetOnQueryFinishedEvent().AddDynamic(this,&USf_EQS_Scheduler::OnQueryFinished);
}

bool USf_EQS_Scheduler::Tick()
{
	const int NumActiveRequests = ActiveRequests.Num();
	const int AllowedNewRequests = FMath::Abs(MaxAllowedActiveRequests-NumActiveRequests) ;
	const int NumSchedulesRequests = ScheduledRequests.Num();
	if (NumSchedulesRequests <=0||AllowedNewRequests<=0)
		return true;
	
	const int NewRequestsToRun  = FMath::Min(NumSchedulesRequests,AllowedNewRequests);
	for (int i = 0; i<NewRequestsToRun ;i++)
	{
		FScheduledEnvRequest Request = ScheduledRequests[0];
		ScheduledRequests.RemoveAt(0);
		RunRequest(Request);
	}

	return true;
}

void USf_EQS_Scheduler::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	
	const FScheduledEnvRequest* RequestPtr =  ActiveRequests.Find(QueryInstance);

	if (RequestPtr==nullptr)
		return;

	ActiveRequests.Remove(QueryInstance);
	FScheduledEnvRequest Request = *RequestPtr;

	TArray<FVector> Locations{};
	QueryInstance->GetQueryResultsAsLocations(Locations);

	FString DebugString = FString::Printf(TEXT("Finished query. Remaining Requests: %id"),ScheduledRequests.Num()); 
	SF_LOG(LogTemp,Type::Warning,DebugString,TP::EQS::Scheduler);
	OnEQSResult.Broadcast(Locations,Request.UniqueIdentifier);
	
}
