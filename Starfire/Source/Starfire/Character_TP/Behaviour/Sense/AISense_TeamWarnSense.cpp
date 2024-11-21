// Fill out your copyright notice in the Description page of Project Settings.


#include "AISense_TeamWarnSense.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Sf_TP_Controller.h"


float UAISense_TeamWarnSense::Update()
{
	for (const FTeamPerceptionEvent& Event : PerceptionEvents)
	{
	}

	return 0.f; 
}

void UAISense_TeamWarnSense::ReportWarnEvent(const UObject* WorldContextObject,AActor* SensedActor,const FAIStimulus& PassedStimulus)
{
	TArray<AActor*> AllAIActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ASf_TP_Controller::StaticClass(), AllAIActors);

	AIPerception::FListenerMap& ListenersMap = *GetListeners();
	
	for (TTuple<FPerceptionListenerID, FPerceptionListener> Element : ListenersMap)
	{
		FPerceptionListener  Listener = Element.Value;
		const AActor* ListenerBodyActor = Listener.GetBodyActor();
		
		//ASf_TP_Controller* AIController = Cast<ASf_TP_Controller>(Element);
		//if (!IsValid(AIController))
		//	continue;

		Element.Value.RegisterStimulus(SensedActor, FAIStimulus(*this, 5.f, SensedActor->GetActorLocation(), ListenerBodyActor->GetActorLocation()));
		
		/*FAIStimulus ModifiedStimulues = PassedStimulus;
		ModifiedStimulues.Type = UAISense_TeamWarnSense::StaticClass();

		UAIPerceptionComponent* PerceptionComponent = AIController->GetAIPerceptionComponent();
		if (!PerceptionComponent)
			continue;
		
		PerceptionComponent->RegisterStimulus(SensedActor,ModifiedStimulues);*/
	}
}

