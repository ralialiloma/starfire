#include "Sf_TP_Controller.h"

#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Touch.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

ASf_TP_Controller::ASf_TP_Controller(const FObjectInitializer& ObjectInitializer)
{
	//AI Perception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void ASf_TP_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASf_TP_Controller::HandlePerception);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ASf_TP_Controller::HandlePerceptionForgotten);
}

void ASf_TP_Controller::HandlePerception(AActor* Actor, FAIStimulus Stimulus)
{
	const TSubclassOf<UAISense> SenseType =  UAIPerceptionSystem::GetSenseClassForStimulus(this,Stimulus);
	
	if (SenseType->IsChildOf(UAISense_Sight::StaticClass()))
		HandleSightPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Hearing::StaticClass()))
		HandleHearingPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Prediction::StaticClass()))
		HandlePredictionPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Touch::StaticClass()))
		HandleTouchPerception(Stimulus);
}

void ASf_TP_Controller::HandleSightPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,Stimulus.WasSuccessfullySensed());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);

	if (!Stimulus.WasSuccessfullySensed())
		return;

	UAISense_Prediction::RequestPawnPredictionEvent(
		GetPawn(),
		USf_FunctionLibrary::GetSfPlayerpawn(this),1);
}

void ASf_TP_Controller::HandleHearingPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,Stimulus.WasSuccessfullySensed());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
}

void ASf_TP_Controller::HandleTouchPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,Stimulus.WasSuccessfullySensed());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
}

void ASf_TP_Controller::HandlePredictionPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,Stimulus.WasSuccessfullySensed());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
}

void ASf_TP_Controller::HandlePerceptionForgotten(AActor* Actor)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::ClearVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation);
}
