
#pragma once

#include "AISense_Alert.h"
#include "Perception/AISenseEvent.h"
#include "AISenseEvent_Alert.generated.h"

UCLASS(MinimalAPI)
class UAISenseEvent_Alert : public UAISenseEvent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense")
	FAlertEvent Event;
	
	virtual FAISenseID GetSenseID() const override;

	FORCEINLINE FAlertEvent GetTeamWarnEvent() const
	{
		//Event.Compile();
		return Event;
	}
};