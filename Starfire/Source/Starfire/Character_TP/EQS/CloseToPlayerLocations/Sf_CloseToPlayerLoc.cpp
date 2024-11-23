
#include "Sf_CloseToPlayerLoc.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/EQS/Sf_AiSettings.h"
#include "Starfire/Character_TP/EQS/Scheduling/Sf_EQS_Scheduler.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


USf_CloseToPlayerLoc::USf_CloseToPlayerLoc(const FObjectInitializer& ObjectInitializer):  Query(nullptr), Player(nullptr),
                                                                                          bFinishedQuery(true),
                                                                                          Scheduler(nullptr)
{
}

void USf_CloseToPlayerLoc::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentCloseToPlayerLocations.Empty();
	
	Query =  USf_AISettings::Get()->CloseToPlayerLocationQuery.LoadSynchronous();
	
	//Register Ticker
	bFinishedQuery = true;
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle,[this]()->void{Tick();},Tickrate,true);
}


void USf_CloseToPlayerLoc::Tick()
{
	if (SHOULD_DEBUG(TP::EQS::CloseToPlayerLocations,EDebugType::Visual))
	{
		for (FVector Location: CurrentCloseToPlayerLocations)
		{
			UKismetSystemLibrary::DrawDebugSphere(this,Location,150.f,6,FColor::Turquoise,Tickrate+0.01f,2);
		}
	}

	Scheduler = USf_EQS_Scheduler::GetCurrent(GetWorld());

	if (!IsValid(Scheduler))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid Scheduler");
		return;
	}
	
	if (!IsValid(Query))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid Query");
		return;
	}

	if(!bFinishedQuery)
		return;
	
	Player =  USf_FunctionLibrary::GetSfPlayerpawn(this);
	if (!IsValid(Player))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid Player");
		return;
	}

	
	const FScheduledEnvRequest ScheduledEnvRequest = FScheduledEnvRequest(Player,Query,EEnvQueryRunMode::AllMatching);

	bFinishedQuery = false;
	Scheduler->OnEQSResult.AddDynamic(this,&USf_CloseToPlayerLoc::OnEQSResult);
	ScheduledEQSID =  Scheduler->ScheduleRequest(ScheduledEnvRequest);
}

USf_CloseToPlayerLoc* USf_CloseToPlayerLoc::GetCurrent(const UWorld* World)
{
	if(!IsValid(World))
		return nullptr;
	
	return  World->GetGameInstance()->GetSubsystem<USf_CloseToPlayerLoc>();
}

TArray<FVector> USf_CloseToPlayerLoc::GetCurrentCloseToPlayerLocations()
{
	return  CurrentCloseToPlayerLocations;
}

void USf_CloseToPlayerLoc::OnEQSResult(const TArray<FVector>& Results, FGuid UniqueID)
{
	if (UniqueID != ScheduledEQSID)
		return;
	

	CurrentCloseToPlayerLocations = Results;
	bFinishedQuery = true;
	Scheduler->OnEQSResult.RemoveDynamic(this,&USf_CloseToPlayerLoc::OnEQSResult);
}


