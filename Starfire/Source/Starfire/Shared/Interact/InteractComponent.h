// Copyright Phoenix Dawn Development LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "InputCoreTypes.h"
#include "Engine/HitResult.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "GenericPlatform/GenericApplication.h"
#include "InteractComponent.generated.h"

#pragma region Enums and Structs
UENUM(Blueprintable)
enum EInteractMoment
{
	Start,
	Tick,
	End
};

UENUM(Blueprintable)
enum class EInteractTriggerType : uint8
{
	None,
	Primary,
	Secondary,
	Other
};

UENUM(BlueprintType)
enum class EInteractionSource : uint8
{
	Component,
	Mouse,
	CenterScreen,
	ParentComponent,
};

UENUM(BlueprintType)
enum EInteractTraceHitType
{
	NoHit,
	Actor,
	Widget,
	Pinpoint,
};

USTRUCT(BlueprintType)
struct FWidgetHitData
{
	GENERATED_BODY()
	
	FWidgetHitData()
		: WidgetHitLocation(FVector2D::ZeroVector)
		, HitWidgetComponent(nullptr)
		, HitWidgetPath()
	{}

	bool IsSame(FWidgetHitData OtherData) const
	{
		if (HitWidgetComponent == OtherData.HitWidgetComponent)
		{
			if (WidgetHitLocation == OtherData.WidgetHitLocation)
			{
				return  true;
			}
		}
		return  false;
	}
	FWidgetPath GetWidgetPath()
	{
		if (HitWidgetPath.IsValid())
		{
			return HitWidgetPath;
		}
		if (HitWidgetComponent)
		{
			HitWidgetPath = FWidgetPath(HitWidgetComponent->GetHitWidgetPath(WidgetHitLocation, false));
		}
		return HitWidgetPath;
	}

	UPROPERTY()
	FVector2D WidgetHitLocation;
	UPROPERTY()
	UWidgetComponent* HitWidgetComponent;

private:
	FWidgetPath HitWidgetPath;

};

USTRUCT(BlueprintType)
struct FInteractTraceResult
{
	GENERATED_BODY()
	
	FInteractTraceResult()
		: HitType(NoHit)
		, HitResult()
		, WidgetHitData()
		, StartLocation(FVector::ZeroVector)
		, EndLocation(FVector::ZeroVector)
	{
	}

	EInteractTraceHitType HitType;
	FHitResult HitResult;
	FWidgetHitData WidgetHitData;
	FVector StartLocation;
	FVector EndLocation;
};

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData()
		: HitType(NoHit)
		, HitActor(nullptr)
		, HitComponent(nullptr)
	{}
	
	enum EDifferenceType
	{
		Same,
		DifferentActor,
		DifferentComponent,
		DifferentType
	};

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EInteractTraceHitType> HitType;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* HitActor;
	
	UPROPERTY(BlueprintReadOnly)
	UPrimitiveComponent* HitComponent;

	UPROPERTY(BlueprintReadOnly)
	FWidgetHitData WidgetHitData;

	EDifferenceType GetDifference(FInteractableData OtherData) const
	{
		if (IsSameActor(OtherData))
		{
			if (IsSameComponent(OtherData))
			{
				if (IsSameType(OtherData))
				{
					return Same;
				}
				return DifferentType;
			}
			return DifferentComponent;
		}
		return DifferentActor;
	}
	
	bool GetWidgetHitData(UWidgetComponent*& WidgetComponent) const
	{
		WidgetComponent = Cast<UWidgetComponent>(HitComponent);
		return WidgetComponent != nullptr;
	}
	bool GetActorHitData(AActor*& ReturnActor) const
	{
		ReturnActor = HitActor;
		return ReturnActor != nullptr;
	}
	bool IsSameComponent(FInteractableData OtherData) const
	{
		if (OtherData.HitComponent == HitComponent)
		{
			return true;
		}
		return  false;
	}
	bool IsSameActor(FInteractableData OtherData) const
	{
		if (OtherData.HitActor == HitActor)
		{
			return true;
		}
		return  false;
	}
	bool IsSameType(FInteractableData OtherData) const
	{
		if (OtherData.HitType == HitType)
		{
			return true;
		}
		return  false;
	}
	bool IsEmpty() const
	{
		if (!HitActor || HitType == EInteractTraceHitType::NoHit)
		{
			return true;
		}
		return  false;
	}
	bool IsPinpoint() const
	{
		if (HitType == EInteractTraceHitType::Pinpoint || HitType == EInteractTraceHitType::Widget)
		{
			return true;
		}
		return  false;
	}
};

#pragma endregion

#pragma region Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighlightTypeChange, EInteractTraceHitType, HitType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHighlightChangeSignature, AActor*, HighlightedActor, UInteractComponent*, InteractComponent, APawn*, TriggeringPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHighlightComponentChangeSignature, UPrimitiveComponent*, InteractedComponent, UInteractComponent*, InteractComponent, APawn*, TriggeringPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractChangeSignature, AActor*, InteractedActor, UInteractComponent*, InteractComponent, APawn*, TriggeringPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FInteractChangeFloatSignature, AActor*, InteractedActor, UInteractComponent*, InteractComponent, APawn*, TriggeringPawn, float, TriggeringSeconds);

#pragma endregion

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARFIRE_API UInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:

#pragma region Delegates
	UPROPERTY(BlueprintAssignable, Category = "Interact|Highlight")
	FHighlightTypeChange OnHighlightTypeChange;

	UPROPERTY(BlueprintAssignable)
	FHighlightChangeSignature OnActorHighlightChange;
	UPROPERTY(BlueprintAssignable)
	FHighlightComponentChangeSignature OnComponentHighlightChange;

	UPROPERTY(BlueprintAssignable)
	FInteractChangeSignature OnPrimaryInteractStartEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnPrimaryInteractTickEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnPrimaryInteractEndEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnPrimaryInteractHoldTickEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeSignature OnPrimaryInteractHoldEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractChangeSignature OnSecondaryInteractStartEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnSecondaryInteractTickEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnSecondaryInteractEndEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeFloatSignature OnSecondaryInteractHoldTickEvent;
	UPROPERTY(BlueprintAssignable)
	FInteractChangeSignature OnSecondaryInteractHoldEvent;
#pragma endregion
	
	//Ctor
	UInteractComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//ActorComponent interface
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Initialize();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableInteract();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableInteract();
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (ExpandBoolAsExecs = ReturnValue))
	bool IsEnabled();

	//Interact Logic
	void InteractLogic();
	void RoutePointer();
	FInteractableData GetNewInteractable(FInteractTraceResult& HitResult) const;
	FInteractTraceResult PerformTrace() const;
	bool GetTracePoints(FVector& Start, FVector& End) const;
	float GetRangePitchMultiplier(const FRotator& Rotator) const;
	
	//Highlight
	void HighlightActorLogic(const FInteractableData& NewHighlight);
	void UnhighlightActorLogic();
	void HighlightComponentLogic(const FInteractableData& NewHighlight);
	void UnhighlightComponentLogic();
	void TriggerHighlightEvents(UObject* Target, bool bHighlight, bool Pinpoint = false);
	
	UFUNCTION(BlueprintCallable, Category = "Highlight")
	void ClearHighlight();

	//Primary Interact
	UFUNCTION(BlueprintCallable)
	void PrimaryInteractLogic(EInteractMoment InteractMoment, float TriggerTime);
	bool CanPrimaryInteract(EInteractMoment InteractMoment);
	void TriggerPrimaryInteractEvents(EInteractMoment InteractMoment, float TriggerTime);
	void TriggerPrimaryInteractEventsOnObject(UObject* Target, EInteractMoment InteractMoment, float TriggerTime);
	void TriggerPrimaryInteractEventsLocal(AActor* Target, EInteractMoment InteractMoment, float TriggerTime);
	void InteractPointer(const EInteractMoment InteractMoment, bool Primary);

	//Secondary Interact
	UFUNCTION(BlueprintCallable)
	void SecondaryInteractLogic(EInteractMoment InteractMoment, float TriggerTime);
	bool CanSecondaryInteract(EInteractMoment InteractMoment);
	void TriggerSecondaryInteractEvents(EInteractMoment InteractMoment, float TriggerTime);
	void TriggerSecondaryInteractEventsOnObject(UObject* Target, EInteractMoment InteractMoment, float TriggerTime);
	void TriggerSecondaryInteractEventsLocal(AActor* Target, EInteractMoment InteractMoment, float TriggerTime);

	//Helpers
	bool CanInteractWithWidget();
	static void ConvertKeyToCharCode(const FKey& Key, bool& bHasKeyCode, uint32& KeyCode, bool& bHasCharCode, uint32& CharCode);
	void GetRelatedComponentsToIgnoreInAutomaticHitTesting(TArray<UPrimitiveComponent*>& IgnorePrimitives) const;
	FInteractableData GetContextData(const EInteractMoment Moment) const;
	bool IsInteractType(EInteractTriggerType InteractType, bool AndNone = true);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult GetLastHitResult();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimitiveComponent* GetHitComponent() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EInteractTriggerType GetCurrentInteractType() const;

	//Global Helpers
	bool HasExceededHold() const;
	bool GetShouldTrace() const;

	//Additional Functionality
	UFUNCTION(BlueprintCallable)
	void FreezeInteract();
	UFUNCTION(BlueprintCallable)
	void UnFreezeInteract();

	UFUNCTION(BlueprintCallable)
	void SwitchInteractSource(EInteractionSource NewInteractSource);

	//Not Fully Built Out (Just a Concept)
	bool RegisterInteract(int32& TriggerID);
	bool UnRegisterInteract(UMETA(Ref) int32& TriggerID);

#pragma region Widget Input
	
	//Widget Interaction
	
	UFUNCTION(BlueprintCallable, Category="Interaction|Widget|Input")
	virtual bool PressKey(FKey Key, bool bRepeat = false);
	
	UFUNCTION(BlueprintCallable, Category="Interaction|Widget|Input")
	virtual bool ReleaseKey(FKey Key);
	
	UFUNCTION(BlueprintCallable, Category="Interaction|Widget|Input")
	virtual bool PressAndReleaseKey(FKey Key);
	
	UFUNCTION(BlueprintCallable, Category="Interaction|Widget|Input")
	virtual bool SendKeyChar(FString Characters, bool bRepeat = false);
	
	UFUNCTION(BlueprintCallable, Category="Interaction|Widget|Input")
	virtual void ScrollWheel(float ScrollDelta);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction|Widget|Set")
	void SetFocus(UWidget* FocusWidget);
	
#pragma endregion

#pragma region Variables
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bEnabled = true;

	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	bool bFrozen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bRequireLocalPawn = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float InteractRange = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float InteractTraceRadius = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float HoldTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bCounteractPitch = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	EInteractionSource InteractSource = EInteractionSource::Component;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(BlueprintReadOnly, Category="Interaction", meta=(ClampMin = "0", ExposeOnSpawn = true))
	int32 VirtualUserIndex = 0;
	static int VirtualUserIndexCount;
	static bool CountScrub;
	
	UPROPERTY(BlueprintReadOnly, Category="Interaction", meta=(ClampMin = "0", UIMin = "0", UIMax = "9", ExposeOnSpawn = true))
	int32 PointerIndex = 0;

protected:

	TSharedPtr<FSlateVirtualUserHandle> VirtualWidgetUser;

	//Current Interactables
	UPROPERTY(BlueprintReadOnly)
	FInteractableData CurrentInteractable;
	UPROPERTY(BlueprintReadOnly)
	FInteractableData ActiveInteractable;
	UPROPERTY(BlueprintReadOnly)
	EInteractTriggerType ActiveInteractType = EInteractTriggerType::None;
	UPROPERTY(BlueprintReadOnly)
	int32 OtherTriggerID = -1;
	UPROPERTY(BlueprintReadOnly)
	float ElapsedTriggerTime = 0;
	UPROPERTY(BlueprintReadOnly)
	bool TriggeredHold = false;
	UPROPERTY()
	FHitResult CurrentHitResult;

	FWidgetHitData PreviousWidgetHitData;
	
	//Keys Pressed
	bool WidgetPressed = false;
	FModifierKeysState ModifierKeys;
	TSet<FKey> PressedKeys;

	//References
	UPROPERTY(BlueprintReadWrite)
	APawn* OwningPawn = nullptr;
	
#pragma  endregion

#pragma region AutomaticInputBinding

public:
	
	UFUNCTION(BlueprintCallable)
	void SetPrimaryInteractInput(UInputAction* NewInputAction);

	UFUNCTION()
	void PrimaryInputActionStart(const FInputActionInstance& ActionInstance);
	UFUNCTION()
	void PrimaryInputActionTick(const FInputActionInstance& ActionInstance);
	UFUNCTION()
	void PrimaryInputActionEnd(const FInputActionInstance& ActionInstance);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	UInputAction* PrimaryInteractInputAction = nullptr;

private:

	//Bindings
	TArray<FInputBindingHandle> PrimaryBindings {};

#pragma endregion

#pragma region Helpers
protected:
	
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "InMoment"), Category = "Interact|Conversion")
	static EInteractMoment ConvertToInteractMoment(EInteractMoment InMoment);
	
#pragma region Debug

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debugging")
	bool bShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debugging")
	bool bLogDebug = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debugging", AdvancedDisplay, meta=( ClampMin = "0.001" ))
	float DebugSphereLineThickness = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debugging", AdvancedDisplay, meta=( ClampMin = "0.001", ClampMax = "50"))
	float DebugLineThickness = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Debugging", AdvancedDisplay)
	FLinearColor DebugColor = FColor::Yellow;

	UFUNCTION(BlueprintCallable, Category="Interaction|Debugging")
	void DebugString(FString Message, FName Key = TEXT("None")) const; 
	
#pragma endregion
	
#pragma endregion
};
