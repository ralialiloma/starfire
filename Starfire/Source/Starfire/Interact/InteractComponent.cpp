#include "InteractComponent.h"
#include "InteractInterfaces.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//TODO: Fix parent space interact

#define LOCTEXT_NAMESPACE "Interact"
UInteractComponent::UInteractComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	bAutoActivate = true;
}

void UInteractComponent::BeginPlay()
{
	OwningPawn = Cast<APawn>(GetOwner());
	Initialize();
	Super::BeginPlay();
}

void UInteractComponent::Initialize_Implementation()
{
	SetComponentTickEnabled(true);
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractLogic();
}

void UInteractComponent::EnableInteract_Implementation()
{
	bEnabled = true;
}

void UInteractComponent::DisableInteract_Implementation()
{
	bEnabled = false;
}

void UInteractComponent::InteractLogic()
{
	if (!bEnabled)
	{
		return;
	}

	FInteractTraceResult HitResult;
	FInteractableData NewInteract = GetNewInteractable(HitResult);
	PreviousWidgetHitData = CurrentInteractable.WidgetHitData;
	CurrentInteractable.WidgetHitData = NewInteract.WidgetHitData;

	CurrentHitResult = HitResult.HitResult;

	switch (CurrentInteractable.GetDifference(NewInteract))
	{
	case FInteractableData::Same:
		break;
	case FInteractableData::DifferentActor:
		UnhighlightActorLogic();
		HighlightActorLogic(NewInteract);
		break;
	case FInteractableData::DifferentComponent:
		UnhighlightComponentLogic();
		HighlightComponentLogic(NewInteract);
		break;
	case FInteractableData::DifferentType:
		if (CurrentInteractable.HitType == Pinpoint)
		{
			UnhighlightComponentLogic();
			HighlightComponentLogic(NewInteract);
		}
		CurrentInteractable.HitType = NewInteract.HitType;
		OnHighlightTypeChange.Broadcast(CurrentInteractable.HitType);
		break;
	}

	if (PreviousWidgetHitData.HitWidgetComponent || CurrentInteractable.WidgetHitData.HitWidgetComponent)
	{
		if (!PreviousWidgetHitData.IsSame(CurrentInteractable.WidgetHitData))
		{
			RoutePointer();
		}
	}
}
void UInteractComponent::RoutePointer()
{
	UWidgetComponent* HitWidget;
	if (CurrentInteractable.GetWidgetHitData(HitWidget))
	{
		HitWidget->RequestRedraw();
	}

	ensure(PointerIndex >= 0);
	FPointerEvent PointerEvent(
		VirtualWidgetUser->GetUserIndex(),
		(uint32) PointerIndex,
		CurrentInteractable.WidgetHitData.WidgetHitLocation,
		PreviousWidgetHitData.WidgetHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);
	
	//Set some bools
	FWidgetPath NewWidgetPathUnderPointer = CurrentInteractable.WidgetHitData.GetWidgetPath();
	if (NewWidgetPathUnderPointer.IsValid())
	{
		check(HitWidget);
		FSlateApplication::Get().RoutePointerMoveEvent(NewWidgetPathUnderPointer, PointerEvent, false);
	}
	else
	{
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);
	}
}

FInteractableData UInteractComponent::GetNewInteractable(FInteractTraceResult& TraceResult) const
{
	TraceResult = PerformTrace();

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		if (TraceResult.HitType == Widget || TraceResult.HitType == Pinpoint)
		{
			UKismetSystemLibrary::DrawDebugLine(this, TraceResult.StartLocation, TraceResult. EndLocation, FColor::Black, 0, DebugLineThickness);
			UKismetSystemLibrary::DrawDebugSphere(this, TraceResult.HitResult.ImpactPoint, 1.f, 12, DebugColor, 0, DebugSphereLineThickness);
		}
	}
#endif

	FInteractableData NewInteract;
	NewInteract.HitType = TraceResult.HitType;
	NewInteract.HitActor = TraceResult.HitResult.GetActor();
	NewInteract.HitComponent = TraceResult.HitResult.GetComponent();
	NewInteract.WidgetHitData = TraceResult.WidgetHitData;
	
	return  NewInteract;
}
FInteractTraceResult UInteractComponent::PerformTrace() const
{
	FInteractTraceResult TraceResult;

	//Return if Tracepoints are invalid
	if (!GetTracePoints(TraceResult.StartLocation, TraceResult.EndLocation))
	{
		return TraceResult;
	}

	//Primary Trace
	FHitResult PrimaryHitResult;
	UKismetSystemLibrary::SphereTraceSingle(
		this,
		TraceResult.StartLocation,
		TraceResult.EndLocation,
		InteractTraceRadius,
		TraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		PrimaryHitResult,
		true,
		FLinearColor::Black,
		DebugColor,
		0.1f
		);

	//Return if primary trace didn't hit anything
	if (!PrimaryHitResult.bBlockingHit)
	{
		TraceResult.HitResult = PrimaryHitResult;
		return TraceResult;
	}

	//Secondary Trace
	TArray<FHitResult> SecondaryHitResults;
	
	TArray<UPrimitiveComponent*> PrimitiveChildren;
	GetRelatedComponentsToIgnoreInAutomaticHitTesting(PrimitiveChildren);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(WidgetInteractionComponentTrace));
	Params.AddIgnoredComponents(PrimitiveChildren);

	GetWorld()->LineTraceMultiByChannel(
		SecondaryHitResults,
		PrimaryHitResult.Location,
		TraceResult.EndLocation,
		UEngineTypes::ConvertToCollisionChannel(TraceChannel),
		Params);

	//Check if not actor interact
	for (const FHitResult& HitResult : SecondaryHitResults)
	{
		//Check if widget Interact
		if (UWidgetComponent* HitWidgetComponent = TraceResult.WidgetHitData.HitWidgetComponent = Cast<UWidgetComponent>(HitResult.GetComponent()))
		{
			if (HitWidgetComponent->IsVisible())
			{
				TraceResult.HitType = Widget;
				TraceResult.HitResult = HitResult;

				if (HitWidgetComponent->GetGeometryMode() == EWidgetGeometryMode::Cylinder)
				{				
					TTuple<FVector, FVector2D> CylinderHitLocation = HitWidgetComponent->GetCylinderHitLocation(TraceResult.HitResult.ImpactPoint, (TraceResult.EndLocation - TraceResult.StartLocation));
					TraceResult.HitResult.ImpactPoint = CylinderHitLocation.Get<0>();
					TraceResult.WidgetHitData.WidgetHitLocation = CylinderHitLocation.Get<1>();
				}
				else
				{
					ensure(HitWidgetComponent->GetGeometryMode() == EWidgetGeometryMode::Plane);
					HitWidgetComponent->GetLocalHitLocation(TraceResult.HitResult.ImpactPoint, TraceResult.WidgetHitData.WidgetHitLocation);
				}
				return TraceResult;
			}
		}
		//Check if Pinpoint Interact
		else if (HitResult.bBlockingHit)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UInteractPinpoint::StaticClass()))
			{
				TraceResult.HitType = Pinpoint;
				TraceResult.HitResult = HitResult;
				return TraceResult;
			}
			break;
		}
	}

	if (UKismetSystemLibrary::DoesImplementInterface(PrimaryHitResult.GetActor(), UInteractPinpoint::StaticClass()))
	{
		TraceResult.HitType = EInteractTraceHitType::NoHit;
		TraceResult.HitResult = PrimaryHitResult;
		return TraceResult;
	}

	//Return standard actor interact
	TraceResult.HitType = Actor;
	TraceResult.HitResult = PrimaryHitResult;
	return TraceResult;
}
bool UInteractComponent::GetTracePoints(FVector& Start, FVector& End) const
{
	FVector WorldDirection;
	Start = FVector::Zero();
	End = FVector::One();
	
	switch(InteractSource)
	{
		//Get standard locations from component transforms.
		case EInteractionSource::Component:
		{
			const FVector WorldLocation = GetComponentLocation();
			const FTransform WorldTransform = GetComponentTransform();
			WorldDirection = WorldTransform.GetUnitAxis(EAxis::X);
				
			Start = WorldLocation;
			End = WorldLocation + WorldDirection * InteractRange * GetRangePitchMultiplier(WorldDirection.Rotation());
			return true;
		}
		//Get ray cast from mouse location.
		case EInteractionSource::Mouse:
		//Get ray cast from screen center.
		case EInteractionSource::CenterScreen:
		{
			const UWorld* World = GetWorld();
			APlayerController* PlayerController = World ? World->GetFirstPlayerController():nullptr;
			if (!PlayerController)
			{
				DebugString("Widget Interaction Component cannot perform trace without a valid PlayerController.");
				return false;
			}
			ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
			
			if (LocalPlayer && LocalPlayer->ViewportClient)
			{
				if (InteractSource == EInteractionSource::Mouse)
				{
					FVector2D MousePosition;
					if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
					{
						FVector WorldOrigin;
						if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldOrigin, WorldDirection) == true)
						{
							Start = WorldOrigin;
							End = WorldOrigin + WorldDirection * InteractRange * GetRangePitchMultiplier(WorldDirection.Rotation());
							return true;
						}
					}
				}
				else if (InteractSource == EInteractionSource::CenterScreen)
				{
					FVector2D ViewportSize;
					LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);

					FVector WorldOrigin;
					if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportSize * 0.5f, WorldOrigin, WorldDirection) == true)
					{
						Start = WorldOrigin;
						End = WorldOrigin + WorldDirection * InteractRange * GetRangePitchMultiplier(WorldDirection.Rotation());
						return true;
					}
				}
			}
			break;
		}
		//Get parent component transforms
		case EInteractionSource::ParentComponent:
		{
			const FVector WorldLocation = GetAttachParent()->GetComponentLocation();
			const FTransform WorldTransform = GetAttachParent()->GetComponentTransform();
			WorldDirection = WorldTransform.GetUnitAxis(EAxis::X);

				
			Start = WorldLocation;
			End = WorldLocation + WorldDirection * InteractRange * GetRangePitchMultiplier(WorldDirection.Rotation());

				UKismetSystemLibrary::DrawDebugArrow(this, Start,  End, 3, FColor::Blue, 0, 1);

				return true;
		}
	}
	return  false;
}
float UInteractComponent::GetRangePitchMultiplier(const FRotator& Rotator) const
{
	if (bCounteractPitch)
	{
		float Pitch = FMath::Clamp(Rotator.Pitch, -90, 0);
		return 1 + (FMath::Abs(Pitch / 90) * 0.7f);
	}
	return 1;
}

void UInteractComponent::HighlightActorLogic(const FInteractableData& NewHighlight)
{
	if (!NewHighlight.IsEmpty())
	{		
		DebugString("Highlight: " + NewHighlight.HitActor->GetName());

		CurrentInteractable = NewHighlight;
		
		TriggerHighlightEvents(CurrentInteractable.HitActor, true, CurrentInteractable.IsPinpoint());
		HighlightComponentLogic(CurrentInteractable);
		OnHighlightTypeChange.Broadcast(CurrentInteractable.HitType);
	}
}
void UInteractComponent::UnhighlightActorLogic()
{
	if (!CurrentInteractable.IsEmpty())
	{
		DebugString("Unhighlight: " + CurrentInteractable.HitActor->GetName());

		if (ActiveInteractType == EInteractTriggerType::Primary)
		{
			PrimaryInteractLogic(End,ElapsedTriggerTime);
		}
		if (ActiveInteractType == EInteractTriggerType::Secondary)
		{
			SecondaryInteractLogic(End,ElapsedTriggerTime);
		}
		
		UnhighlightComponentLogic();
		TriggerHighlightEvents(CurrentInteractable.HitActor, false);
		OnHighlightTypeChange.Broadcast(EInteractTraceHitType::NoHit);
		
		CurrentInteractable = {};
	}
}
void UInteractComponent::HighlightComponentLogic(const FInteractableData& NewHighlight)
{
	if (!NewHighlight.IsEmpty() && NewHighlight.HitComponent)
	{
		CurrentInteractable.HitComponent = NewHighlight.HitComponent;
		CurrentInteractable.HitType = NewHighlight.HitType;
		TriggerHighlightEvents(CurrentInteractable.HitComponent, true, CurrentInteractable.IsPinpoint());
		OnHighlightTypeChange.Broadcast(CurrentInteractable.HitType);
	}
}
void UInteractComponent::UnhighlightComponentLogic()
{
	if (!CurrentInteractable.IsEmpty() && CurrentInteractable.HitComponent)
	{		
		TriggerHighlightEvents(CurrentInteractable.HitComponent, false);
		CurrentInteractable.HitComponent = nullptr;
		OnHighlightTypeChange.Broadcast(CurrentInteractable.HitType);
	}
}


void UInteractComponent::TriggerHighlightEvents(UObject* Target, bool bHighlight, bool Pinpoint)
{
	if (bHighlight)
	{
		if (Target->Implements<UHighlightInterface>())
			IHighlightInterface::Execute_OnHighlight(Target, this, OwningPawn);
	}
	else
	{
		if (Target->Implements<UHighlightInterface>())
			IHighlightInterface::Execute_OnUnhighlight(Target, this, OwningPawn);
	}
}

bool UInteractComponent::CanPrimaryInteract(const EInteractMoment InteractMoment)
{
	if (!bEnabled)
	{
		return false;
	}
	
	FInteractableData ContextInteractableData = GetContextData(InteractMoment);
	
	//If other trigger type
	if (!IsInteractType(EInteractTriggerType::Primary))
	{
		return false;
	}

	//If Interactable is Empty
	if (ContextInteractableData.IsEmpty())
	{
		return false;
	}

	//If Interact Disabled
	if (ContextInteractableData.HitActor->Implements<UPrimaryInteract>())
	{
		if (IPrimaryInteract::Execute_InteractDisabled(ContextInteractableData.HitActor))
			return false;
	}

	return true;
}

void UInteractComponent::PrimaryInteractLogic(const EInteractMoment InteractMoment,const float TriggerTime)
{
	if (!CanPrimaryInteract(InteractMoment))
	{
		return;
	}
	
	ElapsedTriggerTime = TriggerTime;

	if (InteractMoment == Start)
	{
		ActiveInteractable = CurrentInteractable;
		ActiveInteractType = EInteractTriggerType::Primary;
		TriggeredHold = false;
	}

	TriggerPrimaryInteractEvents(InteractMoment, TriggerTime);

	if (InteractMoment == End)
	{
		ActiveInteractable = {};
		ActiveInteractType = EInteractTriggerType::None;
	}
}

void UInteractComponent::TriggerPrimaryInteractEvents(const EInteractMoment InteractMoment, const float TriggerTime)
{
	const FInteractableData Interactable = ActiveInteractable;
	
	TriggerPrimaryInteractEventsOnObject(Interactable.HitActor, InteractMoment, TriggerTime);
	if (Interactable.HitComponent)
		TriggerPrimaryInteractEventsOnObject(Interactable.HitComponent, InteractMoment, TriggerTime);
	
	if (InteractMoment != EInteractMoment::Tick && Interactable.HitType == Widget)
		InteractPointer(InteractMoment, true);
}

void UInteractComponent::TriggerPrimaryInteractEventsOnObject(UObject* Target,const EInteractMoment InteractMoment, const float TriggerTime)
{
	if (!Target->Implements<UPrimaryInteract>())
		return;

	if (IPrimaryInteract::Execute_InteractDisabled(Target))
		return;

	//Debug
	FName Key = static_cast<FName>(Target->GetName()+UEnum::GetValueAsString(InteractMoment));

	switch (InteractMoment)
	{
	case Start:
		IPrimaryInteract::Execute_OnInteractStart(Target, this, OwningPawn);
		break;
	case EInteractMoment::Tick:
		IPrimaryInteract::Execute_OnInteractTick(Target, this, OwningPawn, TriggerTime);

		if (!TriggeredHold)
		{
			float HoldAlpha = FMath::Clamp(TriggerTime/HoldTime,0,1);
			IPrimaryInteract::Execute_OnInteractHoldTick(Target, this, OwningPawn, HoldAlpha);
			
			if (HoldAlpha >= 1)
			{
				IPrimaryInteract::Execute_OnInteractHold(Target, this, OwningPawn);
				TriggeredHold = true;
			}
		}
		break;
	case End:
		IPrimaryInteract::Execute_OnInteractEnd(Target, this, OwningPawn, TriggerTime);
		break;
	}
}

void UInteractComponent::InteractPointer(EInteractMoment InteractMoment, bool Primary)
{
	FKey PrimaryPointer = Primary ? EKeys::LeftMouseButton : EKeys::RightMouseButton;

	if (InteractMoment == Start)
	{
		if (!CanInteractWithWidget())
		{
			return;
		}

		if (WidgetPressed)
		{
			return;
		}
		WidgetPressed = true;
	
		FWidgetPath WidgetPathUnderFinger = CurrentInteractable.WidgetHitData.GetWidgetPath();

		ensure(PointerIndex >= 0);

		FPointerEvent PointerEvent;

		// Find the primary input device for this Slate User
		FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE;
		if (TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(VirtualWidgetUser->GetUserIndex()))
		{
			FPlatformUserId PlatUser = SlateUser->GetPlatformUserId();
			InputDeviceId = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(PlatUser);
		}

		// Just in case there was no input device assigned to this virtual user, get the default platform
		// input device
		if (!InputDeviceId.IsValid())
		{
			InputDeviceId = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
		}
	
		if (PrimaryPointer.IsTouch())
		{
			PointerEvent = FPointerEvent(
				InputDeviceId,
				(uint32)PointerIndex,
				CurrentInteractable.WidgetHitData.WidgetHitLocation,
				PreviousWidgetHitData.WidgetHitLocation,
				1.0f,
				false,
				false,
				false,
				FModifierKeysState(),
				0,
				VirtualWidgetUser->GetUserIndex());		
		}
		else
		{
			PointerEvent = FPointerEvent(
				InputDeviceId,
				(uint32)PointerIndex,
				CurrentInteractable.WidgetHitData.WidgetHitLocation,
				PreviousWidgetHitData.WidgetHitLocation,
				PressedKeys,
				PrimaryPointer,
				0.0f,
				ModifierKeys,
				VirtualWidgetUser->GetUserIndex());
		}
		FReply Reply = FSlateApplication::Get().RoutePointerDownEvent(WidgetPathUnderFinger, PointerEvent);
	}
	else if (InteractMoment == End)
	{
		if ( !CanInteractWithWidget() )
		{
			return;
		}

		if (!WidgetPressed)
		{
			return;
		}
		WidgetPressed = false;
	
		FWidgetPath WidgetPathUnderFinger = ActiveInteractable.WidgetHitData.GetWidgetPath();
		
		ensure(PointerIndex >= 0);
		FPointerEvent PointerEvent;

		// Find the primary input device for this Slate User
		FInputDeviceId InputDeviceId = INPUTDEVICEID_NONE;
		if (TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(VirtualWidgetUser->GetUserIndex()))
		{
			FPlatformUserId PlatUser = SlateUser->GetPlatformUserId();
			InputDeviceId = IPlatformInputDeviceMapper::Get().GetPrimaryInputDeviceForUser(PlatUser);
		}

		// Just in case there was no input device assigned to this virtual user, get the default platform
		// input device
		if (!InputDeviceId.IsValid())
		{
			InputDeviceId = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
		}

		if (PrimaryPointer.IsTouch())
		{
			PointerEvent = FPointerEvent(
				InputDeviceId,
				(uint32)PointerIndex,
				CurrentInteractable.WidgetHitData.WidgetHitLocation,
				PreviousWidgetHitData.WidgetHitLocation,
				0.0f,
				false,
				false,
				false,
				FModifierKeysState(),
				0,
				VirtualWidgetUser->GetUserIndex());
		}
		else
		{
			PointerEvent = FPointerEvent(
				InputDeviceId,
				(uint32)PointerIndex,
				CurrentInteractable.WidgetHitData.WidgetHitLocation,
				PreviousWidgetHitData.WidgetHitLocation,
				PressedKeys,
				PrimaryPointer,
				0.0f,
				ModifierKeys,
				VirtualWidgetUser->GetUserIndex());
		}
		
		FReply Reply = FSlateApplication::Get().RoutePointerUpEvent(WidgetPathUnderFinger, PointerEvent);
		
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);

		FSlateApplication::Get().RoutePointerUpEvent(WidgetPathUnderFinger, PointerEvent);
	}
}

void UInteractComponent::SecondaryInteractLogic(const EInteractMoment InteractMoment, const float TriggerTime)
{
	if (!CanSecondaryInteract(InteractMoment))
	{
		return;
	}
	
	ElapsedTriggerTime = TriggerTime;

	if (InteractMoment == Start)
	{
		ActiveInteractable = CurrentInteractable;
		ActiveInteractType = EInteractTriggerType::Secondary;
		TriggeredHold = false;
	}

	TriggerSecondaryInteractEvents(InteractMoment, TriggerTime);

	if (InteractMoment == End)
	{
		ActiveInteractable = {};
		ActiveInteractType = EInteractTriggerType::None;
	}
}

bool UInteractComponent::CanSecondaryInteract(const EInteractMoment InteractMoment)
{
	if (!bEnabled)
	{
		return false;
	}
	
	FInteractableData ContextInteractableData = GetContextData(InteractMoment);
	
	//If other trigger type
	if (!IsInteractType(EInteractTriggerType::Secondary))
	{
		return false;
	}

	//If Interactable is Empty
	if (ContextInteractableData.IsEmpty())
	{
		return false;
	}

	//If Interact Disabled
	if (ContextInteractableData.HitActor->Implements<USecondaryInteract>())
	{
		if (ISecondaryInteract::Execute_SecondaryInteractDisabled(ContextInteractableData.HitActor))
			return false;
	}

	return true;
}

void UInteractComponent::TriggerSecondaryInteractEvents(const EInteractMoment InteractMoment, const float TriggerTime)
{
	const FInteractableData Interactable = ActiveInteractable;
	
	TriggerSecondaryInteractEventsOnObject(Interactable.HitActor, InteractMoment, TriggerTime);
	if (Interactable.HitComponent)
		TriggerSecondaryInteractEventsOnObject(Interactable.HitComponent, InteractMoment, TriggerTime);
		
	if (InteractMoment != EInteractMoment::Tick && Interactable.HitType == Widget)
		InteractPointer(InteractMoment, false);
}

void UInteractComponent::TriggerSecondaryInteractEventsOnObject(UObject* Target, const EInteractMoment InteractMoment, const float TriggerTime)
{
	if (!Target->Implements<USecondaryInteract>())
		return;
	
	if (ISecondaryInteract::Execute_SecondaryInteractDisabled(Target))
		return;
	
	//Debug
	FName Key = static_cast<FName>(Target->GetName()+UEnum::GetValueAsString(InteractMoment));
	DebugString("Secondary Interact " + UEnum::GetValueAsString(InteractMoment) + ": " + Target->GetName(), Key);

	switch (InteractMoment)
	{
	case Start:
		ISecondaryInteract::Execute_OnSecondaryInteractStart(Target, this, OwningPawn);
		break;
	case EInteractMoment::Tick:
		ISecondaryInteract::Execute_OnSecondaryInteractTick(Target, this, OwningPawn, TriggerTime);
		
		if (!TriggeredHold)
		{
			float HoldAlpha = FMath::Clamp(TriggerTime/HoldTime,0,1);
			ISecondaryInteract::Execute_OnSecondaryInteractHoldTick(Target, this, OwningPawn, HoldAlpha);
			
			if (HoldAlpha >= 1)
			{
				ISecondaryInteract::Execute_OnSecondaryInteractHold(Target, this, OwningPawn);
				TriggeredHold = true;
			}
		}
		break;
	case End:
		ISecondaryInteract::Execute_OnSecondaryInteractEnd(Target, this, OwningPawn, TriggerTime);
		break;
	}
}

void UInteractComponent::GetRelatedComponentsToIgnoreInAutomaticHitTesting(TArray<UPrimitiveComponent*>& IgnorePrimitives) const
{
	TArray<USceneComponent*> SceneChildren;
	if (AActor* Owner = GetOwner())
	{
		if (USceneComponent* Root = Owner->GetRootComponent())
		{
			Root = Root->GetAttachmentRoot();
			Root->GetChildrenComponents(true, SceneChildren);
			SceneChildren.Add(Root);
		}
	}

	for (USceneComponent* SceneComponent : SceneChildren)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(SceneComponent))
		{
			// Don't ignore widget components that are siblings.
			if (SceneComponent->IsA<UWidgetComponent>())
			{
				continue;
			}

			IgnorePrimitives.Add(PrimitiveComponent);
		}
	}
}

FInteractableData UInteractComponent::GetContextData(const TEnumAsByte<EInteractMoment> Moment) const
{
	if (Moment == Start)
	{
		return  CurrentInteractable;
	}
	return ActiveInteractable;
}

bool UInteractComponent::IsInteractType(EInteractTriggerType InteractType, bool AndNone)
{
	if (ActiveInteractType == EInteractTriggerType::None)
	{
		return AndNone;
	}
	return ActiveInteractType == InteractType;
}

FHitResult UInteractComponent::GetLastHitResult()
{
	return  CurrentHitResult;
}

#pragma region WidgetInteraction
bool UInteractComponent::CanInteractWithWidget()
{
	return FSlateApplication::IsInitialized() && VirtualWidgetUser.IsValid();
}
void UInteractComponent::SetFocus(UWidget* FocusWidget)
{
	if (VirtualWidgetUser.IsValid())
	{
		FSlateApplication::Get().SetUserFocus(VirtualWidgetUser->GetUserIndex(), FocusWidget->GetCachedWidget(), EFocusCause::SetDirectly);
	}
}

void UInteractComponent::ClearHighlight()
{
	UnhighlightActorLogic();
}

bool UInteractComponent::ExceededHold() const
{
	return ElapsedTriggerTime > HoldTime;
}

bool UInteractComponent::PressKey(FKey Key, bool bRepeat)
{
	if ( !CanInteractWithWidget() )
	{
		return false;
	}

	bool bHasKeyCode, bHasCharCode;
	uint32 KeyCode, CharCode;
	ConvertKeyToCharCode(Key, bHasKeyCode, KeyCode, bHasCharCode, CharCode);

	FKeyEvent KeyEvent(Key, ModifierKeys, VirtualWidgetUser->GetUserIndex(), bRepeat, CharCode, KeyCode);
	bool bDownResult = FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);

	bool bKeyCharResult = false;
	if (bHasCharCode)
	{
		FCharacterEvent CharacterEvent(CharCode, ModifierKeys, VirtualWidgetUser->GetUserIndex(), bRepeat);
		bKeyCharResult = FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}

	return bDownResult || bKeyCharResult;
}
bool UInteractComponent::ReleaseKey(FKey Key)
{
	if ( !CanInteractWithWidget() )
	{
		return false;
	}

	bool bHasKeyCode, bHasCharCode;
	uint32 KeyCode, CharCode;
	ConvertKeyToCharCode(Key, bHasKeyCode, KeyCode, bHasCharCode, CharCode);

	FKeyEvent KeyEvent(Key, ModifierKeys, VirtualWidgetUser->GetUserIndex(), false, CharCode, KeyCode);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}
void UInteractComponent::ConvertKeyToCharCode(const FKey& Key, bool& bHasKeyCode, uint32& KeyCode, bool& bHasCharCode, uint32& CharCode)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	bHasKeyCode = KeyCodePtr ? true : false;
	bHasCharCode = CharCodePtr ? true : false;

	KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	CharCode = CharCodePtr ? *CharCodePtr : 0;
	
	if (CharCodePtr == nullptr)
	{
		if (Key == EKeys::Tab)
		{
			CharCode = '\t';
			bHasCharCode = true;
		}
		else if (Key == EKeys::BackSpace)
		{
			CharCode = '\b';
			bHasCharCode = true;
		}
		else if (Key == EKeys::Enter)
		{
			CharCode = '\n';
			bHasCharCode = true;
		}
	}
}
bool UInteractComponent::PressAndReleaseKey(FKey Key)
{
	const bool PressResult = PressKey(Key, false);
	const bool ReleaseResult = ReleaseKey(Key);

	return PressResult || ReleaseResult;
}
bool UInteractComponent::SendKeyChar(FString Characters, bool bRepeat)
{
	if ( !CanInteractWithWidget() )
	{
		return false;
	}

	bool bProcessResult = false;

	for ( int32 CharIndex = 0; CharIndex < Characters.Len(); CharIndex++ )
	{
		TCHAR CharKey = Characters[CharIndex];

		FCharacterEvent CharacterEvent(CharKey, ModifierKeys, VirtualWidgetUser->GetUserIndex(), bRepeat);
		bProcessResult |= FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}

	return bProcessResult;
}
void UInteractComponent::ScrollWheel(float ScrollDelta)
{
	if ( !CanInteractWithWidget() )
	{
		return;
	}

	FWidgetPath WidgetPathUnderFinger = ActiveInteractable.WidgetHitData.GetWidgetPath();

	ensure(PointerIndex >= 0);
	FPointerEvent MouseWheelEvent(
		VirtualWidgetUser->GetUserIndex(),
		(uint32)PointerIndex,
		CurrentInteractable.WidgetHitData.WidgetHitLocation,
		PreviousWidgetHitData.WidgetHitLocation,
		PressedKeys,
		EKeys::MouseWheelAxis,
		ScrollDelta,
		ModifierKeys);

	FSlateApplication::Get().RouteMouseWheelOrGestureEvent(WidgetPathUnderFinger, MouseWheelEvent, nullptr);
}
#pragma endregion

#pragma region Helpers
EInteractMoment UInteractComponent::ConvertToInteractMoment(EInteractMoment InMoment)
{
	return  InMoment;
}

void UInteractComponent::DebugString(FString Message, FName Key) const
{
	UKismetSystemLibrary::PrintString(this, Message, bShowDebug, bLogDebug, DebugColor, 1, Key);
}
#pragma endregion

#undef LOCTEXT_NAMESPACE