#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Hearing.h"
#include "AISense_Alert.generated.h"

class UAISenseEvent;
class AActor;
class UAISenseConfig_Alert;


USTRUCT(BlueprintType)
struct FAlertEvent
{
	typedef class UAISense_Alert FSenseClass;
	GENERATED_BODY()

	FVector PerceivedLocation;
	TWeakObjectPtr<AActor> SensedActor;
	TWeakObjectPtr<AActor> SensingActor;
	

	FAlertEvent() : PerceivedLocation(FVector::ZeroVector), SensedActor(nullptr)
	{
	}

	FAlertEvent(AActor* InInstigator,AActor* InSensedActor, const FVector& InPerceivedLocation)
		: PerceivedLocation(InPerceivedLocation), SensedActor(InSensedActor), SensingActor(InInstigator)
	{
	}
};

struct FDigestedAlertProperties
{
public:
	FDigestedAlertProperties();
	FDigestedAlertProperties(const UAISenseConfig_Alert& SenseConfig);
	bool operator==(const FDigestedAlertProperties& Other) const;
	bool operator!=(const FDigestedAlertProperties& Other) const;

public:
	float AlertRadius = 10000;
};


UCLASS(ClassGroup = AI, Config = Game, MinimalAPI)
class UAISense_Alert : public UAISense
{
	GENERATED_BODY()

	
public:
	UAISense_Alert();
	virtual void RegisterWrappedEvent(UAISenseEvent& PerceptionEvent) override;
protected:
	virtual float Update() override;

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "AI|Perception", meta = (WorldContext="WorldContextObject"))
	static void ReportAlertEvent(UObject* WorldContextObject, AActor* SensingActor, AActor* SensedActor);
	void RegisterEvent(const FAlertEvent& Event);
#if WITH_GAMEPLAY_DEBUGGER_MENU
	virtual void DescribeSelfToGameplayDebugger(const UAIPerceptionSystem& PerceptionSystem, FGameplayDebuggerCategory& DebuggerCategory) const;
#endif 
protected:
	void RegisterTeamPerceptionEvent(const FAlertEvent& Event);
	void OnNewListenerImpl(const FPerceptionListener& NewListener);
	void OnListenerRemovedImpl(const FPerceptionListener& RemovedListener);

#pragma endregion

#pragma region Properties
protected:
	TMap<FPerceptionListenerID, FDigestedAlertProperties> DigestedProperties;
	TArray<FAlertEvent> RegisteredEvents;
#pragma endregion
	

};