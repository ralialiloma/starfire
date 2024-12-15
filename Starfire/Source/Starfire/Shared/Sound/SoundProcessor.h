#pragma once

#include "CoreMinimal.h"
#include "SoundProcessor.generated.h"

UCLASS(BlueprintType ,Blueprintable, DefaultToInstanced, EditInlineNew, Abstract)
class USoundProcessor : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyAudio(UAudioComponent* AudioComponent) const;
	
};