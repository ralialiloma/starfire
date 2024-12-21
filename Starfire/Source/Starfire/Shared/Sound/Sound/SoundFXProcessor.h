#pragma once

#include "CoreMinimal.h"
#include "SoundFXProcessor.generated.h"

UCLASS(BlueprintType ,Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class USoundFXProcessor : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyFX(UAudioComponent* AudioComponent) const;
	
};