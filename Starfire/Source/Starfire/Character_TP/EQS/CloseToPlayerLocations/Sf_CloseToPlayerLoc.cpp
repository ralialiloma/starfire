
#include "Sf_CloseToPlayerLoc.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"
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
	UpdateQuery();
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

void USf_CloseToPlayerLoc::RegisterActor(AActor* ActorToRegister)
{
	RegisteredActors.AddUnique(ActorToRegister);
	FCloseToPlayerLocEntry NewEntry;
	NewEntry.Actor = ActorToRegister;
	NewEntry.RegisterdCloseToPlayerPoint = ActorToRegister->GetActorLocation();
	Entries.Add(NewEntry);
	//UpdateEntries();
}

void USf_CloseToPlayerLoc::UnregisterActor(AActor* ActorToUnregister)
{
	RegisteredActors.Remove(ActorToUnregister);
	//UpdateEntries();
}

FVector USf_CloseToPlayerLoc::GetCloseToPlayerLoc(const AActor* ActorToFindLocationFor)
{
	if (!RegisteredActors.Contains(ActorToFindLocationFor))
			return FVector(0,0,0);

	for (const FCloseToPlayerLocEntry Entry: Entries)
	{
		if (Entry.Actor == ActorToFindLocationFor)
			return Entry.RegisterdCloseToPlayerPoint;
	}
	return FVector(0,0,0);
}

bool USf_CloseToPlayerLoc::ValidateCloseToPlayerLoc(FVector Location, float Radius)
{
	for (const FCloseToPlayerLocEntry Entry: Entries)
	{
		if (FVector::Distance(Location,Entry.RegisterdCloseToPlayerPoint)<Radius)
			return  true;
	}

	return false;
}

FVector USf_CloseToPlayerLoc::GetRegisterdActorLocation(const AActor* Actor)
{
	for (const FCloseToPlayerLocEntry Entry: Entries)
	{
		if (Entry.Actor == Actor)
			return  Entry.RegisterdCloseToPlayerPoint;
	}
	return FVector::Zero();
}

void USf_CloseToPlayerLoc::OnEQSResult(const TArray<FVector>& Results, FGuid UniqueID)
{
	if (UniqueID != ScheduledEQSID)
		return;
	

	CurrentCloseToPlayerLocations = Results;
	bFinishedQuery = true;
	UpdateEntries();
	Scheduler->OnEQSResult.RemoveDynamic(this,&USf_CloseToPlayerLoc::OnEQSResult);
}

void USf_CloseToPlayerLoc::UpdateQuery()
{
	if (SHOULD_DEBUG(TP::EQS::CloseToPlayerLocations,EDebugType::Visual))
	{
		for (FVector Location: CurrentCloseToPlayerLocations)
		{
			UKismetSystemLibrary::DrawDebugSphere(this,Location,50.f,6,FColor::Turquoise,Tickrate+0.01f,2);
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



void USf_CloseToPlayerLoc::UpdateEntries()
{
	Player =  USf_FunctionLibrary::GetSfPlayerpawn(this);
	if (!IsValid(Player))
	{
		UDebugFunctionLibrary::Sf_ThrowError(this,"Invalid Player");
		return;
	}
	
	FVector PlayerLocation = Player->GetActorLocation();
	//Sort Locations
	TArray<FCloseToPlayerLocEntry> NewEntries{};

	//CurrentCloseToPlayerLocations.Sort([&ActorLocation, &PlayerLocation](const FVector& A, const FVector& B) {

	//Sort by distance to player ascending
	RegisteredActors.Sort([&PlayerLocation,this](const AActor& A, const AActor& B)
	{
		//return  FVector::DistSquared(GetRegisterdActorLocation(&A),PlayerLocation)< FVector::DistSquared(GetRegisterdActorLocation(&B),PlayerLocation);
		
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		double DistanceA_Actor = 0.f, DistanceB_Actor = 0.f;
		NavSys->GetPathLength(GetWorld(), GetRegisterdActorLocation(&A), PlayerLocation,DistanceA_Actor);
		NavSys->GetPathLength(GetWorld(), GetRegisterdActorLocation(&B), PlayerLocation,DistanceB_Actor);
		
		return  DistanceA_Actor< DistanceB_Actor;
	});

	int Index = 1;
	for (AActor* Actor: RegisteredActors)
	{
		FVector RegisteredActorLocation = GetRegisterdActorLocation(Actor);
		FVector ActorLocation = Actor->GetActorLocation();

		if (CurrentCloseToPlayerLocations.Num()<=0)
		{
			return;
		}

		//Sort by distance to actor and to player ascending
		CurrentCloseToPlayerLocations.Sort([&RegisteredActorLocation, &PlayerLocation,Index,this](const FVector& A, const FVector& B) {
		float WeightAIToPoint = 2*Index;
		constexpr float WeightAIToPlayer = 2;

		const float ScoreA = WeightAIToPoint * FVector::DistSquared(RegisteredActorLocation, A) +
					   WeightAIToPlayer * FVector::DistSquared(PlayerLocation, A);

		const float ScoreB = WeightAIToPoint * FVector::DistSquared(RegisteredActorLocation, B)+
						WeightAIToPlayer * FVector::DistSquared(PlayerLocation, B);
					
			return ScoreA < ScoreB;
		});

		/*TArray<FVector>  TempLocations = CurrentCloseToPlayerLocations;
		for (FVector Location: TempLocations)
		{
			if(UNavigationTargetSubsystem::Get(GetWorld())->HasCloseNavTarget(Location,300.f,Actor))
				CurrentCloseToPlayerLocations.Remove(Location);
		}*/

		if (CurrentCloseToPlayerLocations.Num()<=0)
		{
			continue;
		}

		FVector FoundLocation = CurrentCloseToPlayerLocations[0];
		FCloseToPlayerLocEntry NewEntry = FCloseToPlayerLocEntry();
		NewEntry.Actor = Actor;
		NewEntry.RegisterdCloseToPlayerPoint = FoundLocation;
		CurrentCloseToPlayerLocations.RemoveAt(0);
		float FoundLocationDistanceToPlayer =  FVector::Distance(PlayerLocation,FoundLocation);
	
		TArray<FVector>  TempLocations = CurrentCloseToPlayerLocations;
		for (FVector Location: TempLocations)
		{
			if(FVector::Distance(Location,FoundLocation)<400.f || FVector::Distance(PlayerLocation,Location)<=FoundLocationDistanceToPlayer)
				CurrentCloseToPlayerLocations.Remove(Location);
		}
		
		NewEntries.Add(NewEntry);
		Index++;
	}
	Entries = NewEntries;
}


