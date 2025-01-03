#pragma once

#include "CoreMinimal.h"
#include "VisualFXProcessor.generated.h"

UCLASS(BlueprintType ,Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class UVisualFXProcessor : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyFX(UParticleSystemComponent* FX) const;
	
};