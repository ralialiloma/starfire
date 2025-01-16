// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaskedBackgroundBlur.h"

#include "MaskedBackgroundBlurSlot.h"
#include "HAL/IConsoleManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Rendering/RenderingCommon.h"
#include "Rendering/DrawElements.h"
#include "Brushes/SlateNoResource.h"
#include "SMaskedBackgroundBlur.h"
#include "Components/BackgroundBlurSlot.h"
#include "UObject/EditorObjectVersion.h"
#include "ObjectEditorUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MaskedBackgroundBlur)

#define LOCTEXT_NAMESPACE "UMG"

UMaskedBackgroundBlur::UMaskedBackgroundBlur(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Padding(0.f, 0.f)
	, bApplyAlphaToBlur(true)
	, BlurStrength(0.f)
	, bOverrideAutoRadiusCalculation(false)
	, BlurRadius(0)
	, CornerRadius(0,0,0,0)
	, LowQualityFallbackBrush(FSlateNoResource())
{
	bIsVariable = false;
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UMaskedBackgroundBlur::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMaskedBackgroundBlur.Reset();
}

UClass* UMaskedBackgroundBlur::GetSlotClass() const
{
	return UMaskedBackgroundBlurSlot::StaticClass();
}

TSharedRef<SWidget> UMaskedBackgroundBlur::RebuildWidget()
{
	MyMaskedBackgroundBlur = SNew(SMaskedBackgroundBlur);

	if ( GetChildrenCount() > 0 )
	{
		Cast<UMaskedBackgroundBlurSlot>(GetContentSlot())->BuildSlot(MyMaskedBackgroundBlur.ToSharedRef());
	}
	
	return MyMaskedBackgroundBlur.ToSharedRef();
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void UMaskedBackgroundBlur::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if(MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetPadding(Padding);
		MyMaskedBackgroundBlur->SetHAlign(HorizontalAlignment);
		MyMaskedBackgroundBlur->SetVAlign(VerticalAlignment);

		MyMaskedBackgroundBlur->SetApplyAlphaToBlur(bApplyAlphaToBlur);
		MyMaskedBackgroundBlur->SetBlurRadius(bOverrideAutoRadiusCalculation ? BlurRadius : TOptional<int32>());
		MyMaskedBackgroundBlur->SetBlurStrength(BlurStrength);
		MyMaskedBackgroundBlur->SetLowQualityBackgroundBrush(&LowQualityFallbackBrush);
		MyMaskedBackgroundBlur->SetCornerRadius(CornerRadius);
	}
}

void UMaskedBackgroundBlur::OnSlotAdded(UPanelSlot* InSlot)
{
	UMaskedBackgroundBlurSlot* MaskedBackgroundBlurSlot = CastChecked<UMaskedBackgroundBlurSlot>(InSlot);
	MaskedBackgroundBlurSlot->SetPadding(Padding);
	MaskedBackgroundBlurSlot->SetHorizontalAlignment(HorizontalAlignment);
	MaskedBackgroundBlurSlot->SetVerticalAlignment(VerticalAlignment);

	// Add the child to the live slot if it already exists
	if (MyMaskedBackgroundBlur.IsValid())
	{
		// Construct the underlying slot
		MaskedBackgroundBlurSlot->BuildSlot(MyMaskedBackgroundBlur.ToSharedRef());
	}
}
	
void UMaskedBackgroundBlur::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetContent(SNullWidget::NullWidget);
	}
}

void UMaskedBackgroundBlur::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetPadding(InPadding);
	}
}

FMargin UMaskedBackgroundBlur::GetPadding() const
{
	return Padding;
}

void UMaskedBackgroundBlur::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetHAlign(InHorizontalAlignment);
	}
}

EHorizontalAlignment UMaskedBackgroundBlur::GetHorizontalAlignment() const
{
	return HorizontalAlignment;
}

void UMaskedBackgroundBlur::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetVAlign(InVerticalAlignment);
	}
}

EVerticalAlignment UMaskedBackgroundBlur::GetVerticalAlignment() const
{
	return VerticalAlignment;
}

void UMaskedBackgroundBlur::SetApplyAlphaToBlur(bool bInApplyAlphaToBlur)
{
	bApplyAlphaToBlur = bInApplyAlphaToBlur;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetApplyAlphaToBlur(bInApplyAlphaToBlur);
	}
}

bool UMaskedBackgroundBlur::GetApplyAlphaToBlur() const
{
	return bApplyAlphaToBlur;
}

void UMaskedBackgroundBlur::SetOverrideAutoRadiusCalculation(bool InOverrideAutoRadiusCalculation)
{
	bOverrideAutoRadiusCalculation = InOverrideAutoRadiusCalculation;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		// When set to false, it needs to reset the optional value and it will use the Blur strength to calculate the blur radius
		MyMaskedBackgroundBlur->SetBlurRadius(bOverrideAutoRadiusCalculation ? BlurRadius : TOptional<int32>());
	}	
}

bool UMaskedBackgroundBlur::GetOverrideAutoRadiusCalculation() const
{
	return bOverrideAutoRadiusCalculation;
}

void UMaskedBackgroundBlur::SetBlurRadius(int32 InBlurRadius)
{
	BlurRadius = InBlurRadius;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetBlurRadius(InBlurRadius);
		bOverrideAutoRadiusCalculation = true;
	}
}

int32 UMaskedBackgroundBlur::GetBlurRadius() const
{
	return BlurRadius;
}

void UMaskedBackgroundBlur::SetBlurStrength(float InStrength)
{
	BlurStrength = InStrength;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetBlurStrength(InStrength);
	}
}

float UMaskedBackgroundBlur::GetBlurStrength() const
{
	return BlurStrength;
}

void UMaskedBackgroundBlur::SetCornerRadius(FVector4 InCornerRadius)
{
	CornerRadius = InCornerRadius;
	if (MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetCornerRadius(InCornerRadius);
	}
}

FVector4 UMaskedBackgroundBlur::GetCornerRadius() const
{
	return CornerRadius;
}

void UMaskedBackgroundBlur::SetLowQualityFallbackBrush(const FSlateBrush& InBrush)
{
	LowQualityFallbackBrush = InBrush;
	if(MyMaskedBackgroundBlur.IsValid())
	{
		MyMaskedBackgroundBlur->SetLowQualityBackgroundBrush(&LowQualityFallbackBrush);
	}
}

FSlateBrush UMaskedBackgroundBlur::GetLowQualityFallbackBrush() const
{
	return LowQualityFallbackBrush;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS

void UMaskedBackgroundBlur::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);
}

void UMaskedBackgroundBlur::PostLoad()
{
	Super::PostLoad();

	if ( GetLinkerCustomVersion(FEditorObjectVersion::GUID) < FEditorObjectVersion::AddedBackgroundBlurContentSlot )
	{
		//Convert existing slot to new background blur slot slot.
		if ( UPanelSlot* PanelSlot = GetContentSlot() )
		{
			if ( PanelSlot->IsA<UMaskedBackgroundBlurSlot>() == false )
			{
				UMaskedBackgroundBlurSlot* BlurSlot = NewObject<UMaskedBackgroundBlurSlot>(this);
				BlurSlot->Content = PanelSlot->Content;
				BlurSlot->Content->Slot = BlurSlot;
				BlurSlot->Parent = this;
				Slots[0] = BlurSlot;

				// We don't want anyone considering this panel slot for anything, so mark it pending kill.  Otherwise
				// it will confuse the pass we do when doing template validation when it finds it outered to the blur widget.
				PanelSlot->MarkAsGarbage();
			}
		}
	}
}

#if WITH_EDITOR

void UMaskedBackgroundBlur::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static bool IsReentrant = false;

	if (!IsReentrant)
	{
		IsReentrant = true;

		if (PropertyChangedEvent.Property)
		{
			static const FName PaddingName("Padding");
			static const FName HorizontalAlignmentName("HorizontalAlignment");
			static const FName VerticalAlignmentName("VerticalAlignment");

			FName PropertyName = PropertyChangedEvent.Property->GetFName();

			if (UMaskedBackgroundBlurSlot* BlurSlot = Cast<UMaskedBackgroundBlurSlot>(GetContentSlot()))
			{
				if (PropertyName == PaddingName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, PaddingName, BlurSlot, PaddingName);
				}
				else if (PropertyName == HorizontalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, HorizontalAlignmentName, BlurSlot, HorizontalAlignmentName);
				}
				else if (PropertyName == VerticalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, VerticalAlignmentName, BlurSlot, VerticalAlignmentName);
				}
			}
		}

		IsReentrant = false;
	}
}

const FText UMaskedBackgroundBlur::GetPaletteCategory()
{
	return LOCTEXT("SpecialFX", "Special Effects");
}

#endif


#undef LOCTEXT_NAMESPACE