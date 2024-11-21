#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "GenericTeamAgentInterface.h"
#include "AISense_TeamWarnSense.generated.h"

class UAISenseEvent;
class AActor;

USTRUCT(BlueprintType)
struct FTeamPerceptionEvent
{
	GENERATED_BODY()
	
	FVector PerceivedLocation;
	
	TWeakObjectPtr<AActor> Instigator;
	
	FGenericTeamId TeamIdentifier;

	/** Range at which the event is considered perceived by team members */
	float NotificationRange;

	FTeamPerceptionEvent() : PerceivedLocation(FVector::ZeroVector), TeamIdentifier(FGenericTeamId()), NotificationRange(1000.f) {}

	FTeamPerceptionEvent(AActor* InInstigator, const FVector& InPerceivedLocation, FGenericTeamId InTeamIdentifier, const float InNotificationRange = 1000.f)
		: Instigator(InInstigator), PerceivedLocation(InPerceivedLocation), TeamIdentifier(InTeamIdentifier), NotificationRange(InNotificationRange)
	{}
};

UCLASS(ClassGroup = AI, Config = Game, MinimalAPI)
class UAISense_TeamWarnSense : public UAISense
{
	GENERATED_BODY()

protected:
	TArray<FTeamPerceptionEvent> PerceptionEvents;

public:
	virtual float Update() override;
	
	void ReportWarnEvent(const UObject* WorldContextObject, AActor* SensedActor, const FAIStimulus& PassedStimulus);
protected:
	void RegisterTeamPerceptionEvent(const FTeamPerceptionEvent& Event);
	

};