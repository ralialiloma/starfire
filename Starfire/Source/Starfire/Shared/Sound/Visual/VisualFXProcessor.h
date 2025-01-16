#pragma once

#include "CoreMinimal.h"
#include "VisualFXProcessor.generated.h"

class UFXSystemComponent;

UCLASS(BlueprintType ,Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class UVisualFXProcessor : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyFX(UFXSystemComponent* FX) const;
	
};