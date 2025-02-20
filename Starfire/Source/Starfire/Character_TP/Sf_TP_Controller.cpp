#include "Sf_TP_Controller.h"

#include "Sf_PathFollowingComponent.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Behaviour/AlertSense/AISense_Alert.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Touch.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(Sf_Log_TP_Controller, Display, Display);

ASf_TP_Controller::ASf_TP_Controller(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<USf_PathFollowingComponent>(TEXT("PathFollowingComponent")))
{
	//AI Perception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	FGenericTeamId::SetAttitudeSolver(&ASf_TP_Controller::GetAttitude);
}

void ASf_TP_Controller::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASf_TP_Controller::HandlePerception);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ASf_TP_Controller::HandlePerceptionForgotten);
}

FPathFollowingRequestResult ASf_TP_Controller::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
	const FPathFollowingRequestResult Result =  Super::MoveTo(MoveRequest, OutPath);
	return Result;
}

void ASf_TP_Controller::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!IsValid(GetWorld()))
		return;

	if (!IsValid(GetWorld()->GetGameInstance()))
		return;
	
	if (!IsValid(InPawn))
	{
		//UE_LOG(Sf_Log_TP_Controller, Error, TEXT("Missing Pawn"))
		return;
	}
	
	TP_Character = Cast<ASf_TP_Character>(InPawn);
	if (!IsValid(TP_Character))
	{
		UE_LOG(
			Sf_Log_TP_Controller,
			Error,
			TEXT("Compatible pawn must be of type %s but is of type %s"),
			*ASf_TP_Character::StaticClass()->GetName(),
			*InPawn->GetClass()->GetName());
		return;
	}
	TP_Character->GetSfDamageController()->OnDamageReceived_BP.AddDynamic(this, &ASf_TP_Controller::OnReceiveDamage);
}



ASf_TP_Character* ASf_TP_Controller::GetTP_Character()
{
	return TP_Character;
}

ETeamAttitude::Type ASf_TP_Controller::GetAttitude(const FGenericTeamId Of, const FGenericTeamId Towards)
{
	if (Of == Towards)
	{
		return ETeamAttitude::Friendly;
	}
	
	return ETeamAttitude::Hostile;
}

void ASf_TP_Controller::HandlePerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (!(Actor->GetClass()->IsChildOf(ASf_FP_Character::StaticClass())))
		return;
	
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	bool bHasSensedAlready =  UBlackboardKeyHelperLibrary::GetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer);
	
	const TSubclassOf<UAISense> SenseType =  UAIPerceptionSystem::GetSenseClassForStimulus(this,Stimulus);
	
	if (SenseType->IsChildOf(UAISense_Sight::StaticClass()))
		HandleSightPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Hearing::StaticClass()))
		HandleHearingPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Prediction::StaticClass()))
		HandlePredictionPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Touch::StaticClass()))
		HandleTouchPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Damage::StaticClass()))
		HandleTouchPerception(Stimulus);
	else if (SenseType->IsChildOf(UAISense_Alert::StaticClass()))
		HandleTeamPerception(Stimulus);
	
	if (!SenseType->IsChildOf(UAISense_Alert::StaticClass()) )
		UAISense_Alert::ReportAlertEvent(this,GetPawn(),Actor);
		

	if (Stimulus.WasSuccessfullySensed())
	{
		if (!bHasSensedAlready)
		{
			SF_PRINT_TO_SCREEN(-1,2,FColor::Red,TEXT("Successfully Sensed Player"),TP::Controller)
			OnPlayerDetected_CPP.Broadcast();
			OnPlayerDetected_BP.Broadcast();
		}
	}

}

void ASf_TP_Controller::HandleSightPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}

	if (!Stimulus.WasSuccessfullySensed())
		return;

	UAISense_Prediction::RequestPawnPredictionEvent(
		GetPawn(),
		USf_FunctionLibrary::GetSfPlayerpawn(this),1);
}

void ASf_TP_Controller::HandleHearingPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}
}

void ASf_TP_Controller::HandleTouchPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}
}

void ASf_TP_Controller::HandlePredictionPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}
}

void ASf_TP_Controller::HandleDamagePerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}
}

void ASf_TP_Controller::HandleTeamPerception(const FAIStimulus& Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,true);
		UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation,Stimulus.StimulusLocation);
	}
}

void ASf_TP_Controller::HandlePerceptionForgotten(AActor* Actor)
{
	SF_PRINT_TO_SCREEN(-1,2,FColor::Red,TEXT("Forgot Player"),TP::Controller)
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,EBoolBlackboardKey::SensedPlayer,false);
	UBlackboardKeyHelperLibrary::ClearVectorValue(BlackboardComponent,ELocationBlackboardKey::LastPlayerLocation);
	OnPlayerForgotten_CPP.Broadcast();
	OnPlayerForgotten_BP.Broadcast();
}


void ASf_TP_Controller::OnReceiveDamage(
	float RemainingHealth,
	float DamageReceived,
	FVector HitLocation,
	FVector HitNormal,
	FGameplayTag DamageType)
{
	UAISense_Damage::ReportDamageEvent(
		this,
		TP_Character,USf_FunctionLibrary::GetSfPlayerpawn(this),
		DamageReceived,
		HitLocation,
		HitLocation);
}
