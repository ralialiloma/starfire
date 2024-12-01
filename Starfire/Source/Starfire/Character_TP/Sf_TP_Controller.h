#pragma once
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "Sf_TP_Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "Sf_TP_Controller.generated.h"

UCLASS()
class ASf_TP_Controller: public AAIController
{
	GENERATED_BODY()
public:
	ASf_TP_Controller(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
protected:
	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr) override;
	virtual void SetPawn(APawn* InPawn) override;


#pragma region Functions
public:
protected:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "AI")
	ASf_TP_Character* GetTP_Character();

	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);

private:
	UFUNCTION()
	void HandlePerception(AActor* Actor,FAIStimulus Stimulus);
	void HandleSightPerception(const FAIStimulus& Stimulus);
	void HandleHearingPerception(const FAIStimulus& Stimulus);
	void HandleTouchPerception(const FAIStimulus& Stimulus);
	void HandlePredictionPerception(const FAIStimulus& Stimulus);
	void HandleDamagePerception(const FAIStimulus& Stimulus);
	void HandleTeamPerception(const FAIStimulus& Stimulus);

	UFUNCTION()
	void HandlePerceptionForgotten(AActor* Actor);
	UFUNCTION()
	void OnReceiveDamage(float RemainingHealth, float DamageReceived, FVector HitLocation, FVector HitNormal, FGameplayTag DamageType);
#pragma endregion

#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
private:
	UPROPERTY()
	ASf_TP_Character* TP_Character;
#pragma endregion
};
