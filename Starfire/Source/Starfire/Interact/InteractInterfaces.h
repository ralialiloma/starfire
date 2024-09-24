#pragma once

#include "CoreMinimal.h"
#include "InteractComponent.h"
#include "UObject/Interface.h"
#include "InteractInterfaces.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractPinpoint : public UInterface
{
	GENERATED_BODY()
};

class IInteractPinpoint
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FHighlightSignature, UInteractComponent*, APawn*);

UINTERFACE(MinimalAPI, Blueprintable)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

class IHighlightInterface
{
	GENERATED_BODY()

public:
	FHighlightSignature OnHighlightEvent;
	FHighlightSignature OnUnhighlightEvent;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Highlight")
	void OnHighlight(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Highlight")
	void OnUnhighlight(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Highlight|Additive")
	bool HighlightDisabled();

	virtual void OnHighlight_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnHighlightEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
	virtual void OnUnhighlight_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnUnhighlightEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FInteractSignature, UInteractComponent*, APawn*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FInteractFloatSignature, UInteractComponent*, APawn*, float);

UINTERFACE(MinimalAPI, Blueprintable)
class UPrimaryInteract : public UInterface
{
	GENERATED_BODY()
};

class IPrimaryInteract
{
	GENERATED_BODY()

public:
	FInteractSignature OnPrimaryInteractStartEvent;
	FInteractFloatSignature OnPrimaryInteractTickEvent;
	FInteractFloatSignature OnPrimaryInteractEndEvent;
	FInteractFloatSignature OnPrimaryInteractHoldTickEvent;
	FInteractSignature OnPrimaryInteractHoldEvent;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact")
	void OnInteractStart(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact")
	void OnInteractTick(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact")
	void OnInteractEnd(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact|Additive")
	void OnInteractHoldTick(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float Alpha);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact|Additive")
	void OnInteractHold(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Primary Interact|Additive")
	bool InteractDisabled();

	virtual void OnInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnPrimaryInteractStartEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
	virtual void OnInteractTick_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds)
	{
		OnPrimaryInteractTickEvent.Broadcast(InteractComponent, TriggeringPawn, TriggeredSeconds);
	}
	virtual void OnInteractEnd_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds)
	{
		OnPrimaryInteractEndEvent.Broadcast(InteractComponent, TriggeringPawn, TriggeredSeconds);
	}
	virtual void OnInteractHoldTick_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float Alpha)
	{
		OnPrimaryInteractHoldTickEvent.Broadcast(InteractComponent, TriggeringPawn, Alpha);
	}
	virtual void OnInteractHold_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnPrimaryInteractHoldEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
};

UINTERFACE(MinimalAPI, Blueprintable)
class USecondaryInteract : public UInterface
{
	GENERATED_BODY()
};

class ISecondaryInteract
{
	GENERATED_BODY()

public:
	FInteractSignature OnSecondaryInteractStartEvent;
	FInteractFloatSignature OnSecondaryInteractTickEvent;
	FInteractFloatSignature OnSecondaryInteractEndEvent;
	FInteractFloatSignature OnSecondaryInteractHoldTickEvent;
	FInteractSignature OnSecondaryInteractHoldEvent;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact")
	void OnSecondaryInteractStart(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact")
	void OnSecondaryInteractTick(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact")
	void OnSecondaryInteractEnd(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact|Additive")
	void OnSecondaryInteractHoldTick(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float Alpha);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact|Additive")
	void OnSecondaryInteractHold(UInteractComponent* InteractComponent, APawn* TriggeringPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Secondary Interact|Additive")
	bool SecondaryInteractDisabled();

	virtual void OnSecondaryInteractStart_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnSecondaryInteractStartEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
	virtual void OnSecondaryInteractTick_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds)
	{
		OnSecondaryInteractTickEvent.Broadcast(InteractComponent, TriggeringPawn, TriggeredSeconds);
	}
	virtual void OnSecondaryInteractEnd_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float TriggeredSeconds)
	{
		OnSecondaryInteractEndEvent.Broadcast(InteractComponent, TriggeringPawn, TriggeredSeconds);
	}
	virtual void OnSecondaryInteractHoldTick_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn, float Alpha)
	{
		OnSecondaryInteractHoldTickEvent.Broadcast(InteractComponent, TriggeringPawn, Alpha);
	}
	virtual void OnSecondaryInteractHold_Implementation(UInteractComponent* InteractComponent, APawn* TriggeringPawn)
	{
		OnSecondaryInteractHoldEvent.Broadcast(InteractComponent, TriggeringPawn);
	}
};


