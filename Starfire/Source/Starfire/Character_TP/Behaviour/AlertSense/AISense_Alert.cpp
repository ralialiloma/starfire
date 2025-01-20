// Fill out your copyright notice in the Description page of Project Settings.


#include "AISense_Alert.h"

#include "AISenseConfig_Alert.h"
#include "AISenseEvent_Alert.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"

FDigestedAlertProperties::FDigestedAlertProperties(const UAISenseConfig_Alert& SenseConfig)
{
	AlertRadius = SenseConfig.Radius;
}

bool FDigestedAlertProperties::operator==(const FDigestedAlertProperties& Other) const
{
	return FMath::IsNearlyEqual(AlertRadius, Other.AlertRadius);
}

bool FDigestedAlertProperties::operator!=(const FDigestedAlertProperties& Other) const
{
	return !(*this == Other);
}

UAISense_Alert::UAISense_Alert()
{
	OnNewListenerDelegate.BindUObject(this, &UAISense_Alert::OnNewListenerImpl);
	OnListenerRemovedDelegate.BindUObject(this, &UAISense_Alert::OnListenerRemovedImpl);
}

float UAISense_Alert::Update()
{
	AIPerception::FListenerMap& ListenersMap = *GetListeners();
	
	for (const FAlertEvent& Event : RegisteredEvents)
	{
		if (!Event.SensingActor.IsValid() || !Event.SensedActor.IsValid())
			continue;

		//const AActor* SensedActor = Event.SensedActor.Get();
		//const FVector SensedActorLocation = SensedActor->GetActorLocation();
		const AActor* SensingActor = Event.SensingActor.Get();
		const FVector SensingActorLocation = SensingActor->GetActorLocation();
		
		for (auto& Elem : ListenersMap)
		{
			FPerceptionListener Listener = Elem.Value;
			const AActor* ListenerBodyActor = Listener.GetBodyActor();
			const FVector BodyActorLocation = ListenerBodyActor->GetActorLocation();

			if (SensingActor == ListenerBodyActor)
				continue;
			
			for (int DigestedPropertyIndex = 0; DigestedPropertyIndex < DigestedProperties.Num(); DigestedPropertyIndex++)
			{
				TArray<FPerceptionListenerID> PerceptionListenerIDs{};
				DigestedProperties.GetKeys(PerceptionListenerIDs);
				const FDigestedAlertProperties* DigestedAlertPropertyRef =  DigestedProperties.Find(PerceptionListenerIDs[DigestedPropertyIndex]);
				if (!DigestedAlertPropertyRef)
					continue;
				const FDigestedAlertProperties DigestedAlertProperty = *DigestedAlertPropertyRef;
				
				if (FVector::Distance(BodyActorLocation,SensingActorLocation)>DigestedAlertProperty.AlertRadius)
					continue;

				/*FString DebugString = FString::Printf(
					TEXT("ListenerBody: %s \n SensingActor %s \n Distance is less then %f"),
					*ListenerBodyActor->GetActorLabel(),
					*SensingActor->GetActorLabel());
				
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, DebugString);*/
				
				Elem.Value.RegisterStimulus(
					Event.SensedActor.Get(),
					FAIStimulus(
						*this,
						5.f,
						Event.SensedActor->GetActorLocation(),
						Event.SensingActor->GetActorLocation()));
			}
		}
	}
	
	RegisteredEvents.Reset();
	return SuspendNextUpdate;
}

void UAISense_Alert::RegisterEvent(const FAlertEvent& Event)
{
	RegisteredEvents.Add(Event);
	RequestImmediateUpdate();
}

#if WITH_GAMEPLAY_DEBUGGER_MENU
void UAISense_Alert::DescribeSelfToGameplayDebugger(const UAIPerceptionSystem& PerceptionSystem, FGameplayDebuggerCategory& DebuggerCategory) const
{
	Super::DescribeSelfToGameplayDebugger(PerceptionSystem, DebuggerCategory);
}
#endif 

void UAISense_Alert::OnNewListenerImpl(const FPerceptionListener& NewListener)
{
	check(NewListener.Listener.IsValid());
	//Get the config
	UAISenseConfig* Config = NewListener.Listener->GetSenseConfig(GetSenseID());
	const UAISenseConfig_Alert* SenseConfig = Cast<UAISenseConfig_Alert>(Config);
	check(SenseConfig);
	//Consume properties from the sense config
	FDigestedAlertProperties PropertyDigest(*SenseConfig);
	DigestedProperties.Add( NewListener.GetListenerID(),PropertyDigest);
	RequestImmediateUpdate();
}

void UAISense_Alert::OnListenerRemovedImpl(const FPerceptionListener& RemovedListener)
{
	DigestedProperties.FindAndRemoveChecked(RemovedListener.GetListenerID());
}


void UAISense_Alert::RegisterWrappedEvent(UAISenseEvent& PerceptionEvent)
{
	Super::RegisterWrappedEvent(PerceptionEvent);
	UAISenseEvent_Alert* Alert = Cast<UAISenseEvent_Alert>(&PerceptionEvent);
	ensure(Alert);
	if (Alert)
	{
		RegisterEvent(Alert->GetTeamWarnEvent());
	}
}



void UAISense_Alert::ReportAlertEvent(UObject* WorldContextObject,AActor* SensingActor,AActor* SensedActor)
{
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(WorldContextObject);
	if (PerceptionSystem)
	{
		const FAlertEvent WarnFriendsEvent =  FAlertEvent(SensingActor,SensedActor, SensedActor->GetActorLocation());
		PerceptionSystem->OnEvent(WarnFriendsEvent);
	}
}




