// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaskedBackgroundBlurSlot.h"
#include "ObjectEditorUtils.h"
#include "SMaskedBackgroundBlur.h"
#include "MaskedBackgroundBlur.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MaskedBackgroundBlurSlot)

/////////////////////////////////////////////////////
// UMaskedBackgroundBlurSlot

UMaskedBackgroundBlurSlot::UMaskedBackgroundBlurSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Padding = FMargin(4.f, 2.f);

	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UMaskedBackgroundBlurSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MaskedBackgroundBlur.Reset();
}

void UMaskedBackgroundBlurSlot::BuildSlot(TSharedRef<SMaskedBackgroundBlur> InMaskedBackgroundBlur)
{
	MaskedBackgroundBlur = InMaskedBackgroundBlur;

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	MaskedBackgroundBlur->SetPadding(Padding);
	MaskedBackgroundBlur->SetHAlign(HorizontalAlignment);
	MaskedBackgroundBlur->SetVAlign(VerticalAlignment);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

	MaskedBackgroundBlur->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
FMargin UMaskedBackgroundBlurSlot::GetPadding() const
{
	return Padding;
}

void UMaskedBackgroundBlurSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	CastChecked<UMaskedBackgroundBlur>(Parent)->SetPadding(InPadding);
}

EHorizontalAlignment UMaskedBackgroundBlurSlot::GetHorizontalAlignment() const
{
	return HorizontalAlignment;
}

void UMaskedBackgroundBlurSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	CastChecked<UMaskedBackgroundBlur>(Parent)->SetHorizontalAlignment(InHorizontalAlignment);
}

EVerticalAlignment UMaskedBackgroundBlurSlot::GetVerticalAlignment() const
{
	return VerticalAlignment;
}

void UMaskedBackgroundBlurSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	CastChecked<UMaskedBackgroundBlur>(Parent)->SetVerticalAlignment(InVerticalAlignment);
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

void UMaskedBackgroundBlurSlot::SynchronizeProperties()
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if ( MaskedBackgroundBlur.IsValid() )
	{
		SetPadding(Padding);
		SetHorizontalAlignment(HorizontalAlignment);
		SetVerticalAlignment(VerticalAlignment);
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

#if WITH_EDITOR

void UMaskedBackgroundBlurSlot::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static bool IsReentrant = false;

	if ( !IsReentrant )
	{
		IsReentrant = true;

		if ( PropertyChangedEvent.Property )
		{
			static const FName PaddingName("Padding");
			static const FName HorizontalAlignmentName("HorizontalAlignment");
			static const FName VerticalAlignmentName("VerticalAlignment");

			FName PropertyName = PropertyChangedEvent.Property->GetFName();

			if ( UMaskedBackgroundBlur* ParentMaskedBackgroundBlur = CastChecked<UMaskedBackgroundBlur>(Parent) )
			{
				if (PropertyName == PaddingName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, PaddingName, ParentMaskedBackgroundBlur, PaddingName);
				}
				else if (PropertyName == HorizontalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, HorizontalAlignmentName, ParentMaskedBackgroundBlur, HorizontalAlignmentName);
				}
				else if (PropertyName == VerticalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, VerticalAlignmentName, ParentMaskedBackgroundBlur, VerticalAlignmentName);
				}
			}
		}

		IsReentrant = false;
	}
}

#endif