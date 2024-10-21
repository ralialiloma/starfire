#include "Sf_TP_Controller.h"
#include "Perception/AIPerceptionComponent.h"

ASf_TP_Controller::ASf_TP_Controller(const FObjectInitializer& ObjectInitializer)
{
	//AI Perception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}
