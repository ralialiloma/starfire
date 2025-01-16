// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/ScriptMacros.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/PanelSlot.h"
#include "Layout/Margin.h"
#include "MaskedBackgroundBlurSlot.generated.h"


class SMaskedBackgroundBlur;
class UMaskedBackgroundBlur;

/**
 * The Slot for the UMaskedBackgroundBlurSlot, contains the widget displayed in a MaskedBackgroundBlur's single slot
 */
UCLASS(MinimalAPI)
class UMaskedBackgroundBlurSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:

	STARFIRE_API FMargin GetPadding() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Background Blur Slot")
	STARFIRE_API void SetPadding(FMargin InPadding);

	STARFIRE_API EHorizontalAlignment GetHorizontalAlignment() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Background Blur Slot")
	STARFIRE_API void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	STARFIRE_API EVerticalAlignment GetVerticalAlignment() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Background Blur Slot")
	STARFIRE_API void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

protected:
	UE_DEPRECATED(5.1, "Direct access to Padding is deprecated. Please use the getter or setter.")
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, BlueprintSetter="SetPadding", Category="Layout|Background Blur Slot")
	FMargin Padding;

	UE_DEPRECATED(5.1, "Direct access to HorizontalAlignment is deprecated. Please use the getter or setter.")
	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, BlueprintSetter="SetHorizontalAlignment", Category="Layout|Background Blur Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UE_DEPRECATED(5.1, "Direct access to VerticalAlignment is deprecated. Please use the getter or setter.")
	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, BlueprintSetter="SetVerticalAlignment", Category="Layout|Background Blur Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:

	//~ Begin UPanelSlot interface
	STARFIRE_API virtual void SynchronizeProperties() override;
	//~ End of UPanelSlot interface

	/** Builds the underlying slot for the slate MaskedBackgroundBlur. */
	STARFIRE_API void BuildSlot(TSharedRef<SMaskedBackgroundBlur> InMaskedBackgroundBlur);

	STARFIRE_API virtual void ReleaseSlateResources(bool bReleaseChildren) override;

public:

#if WITH_EDITOR

	//~ Begin UObject interface
	STARFIRE_API virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End of UObject interface

#endif

private:

	/** A pointer to the MaskedBackgroundBlur to allow us to adjust the size, padding...etc at runtime. */
	TSharedPtr<SMaskedBackgroundBlur> MaskedBackgroundBlur;

	friend UMaskedBackgroundBlur;
};
