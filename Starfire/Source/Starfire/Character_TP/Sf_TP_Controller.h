#pragma once
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Sf_TP_Controller.generated.h"

UCLASS()
class ASf_TP_Controller: public AAIController
{
	GENERATED_BODY()
public:
	ASf_TP_Controller(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
#pragma region Functions
public:
private:
	UFUNCTION()
	void HandlePerception(AActor* Actor,FAIStimulus Stimulus);
	void HandleSightPerception(const FAIStimulus& Stimulus);
	void HandleHearingPerception(const FAIStimulus& Stimulus);
	void HandleTouchPerception(const FAIStimulus& Stimulus);
	void HandlePredictionPerception(const FAIStimulus& Stimulus);

	UFUNCTION()
	void HandlePerceptionForgotten(AActor* Actor);
#pragma endregion

#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
#pragma endregion
};
