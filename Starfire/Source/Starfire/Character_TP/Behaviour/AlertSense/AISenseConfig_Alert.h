#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig.h"
#include "AISenseConfig_Alert.generated.h"

UCLASS(meta = (DisplayName = "AI Alert config"))
class UAISenseConfig_Alert : public UAISenseConfig
{
	GENERATED_BODY()

public:
	UAISenseConfig_Alert(const FObjectInitializer& ObjectInitializer);
	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, config)
	float Radius;
};