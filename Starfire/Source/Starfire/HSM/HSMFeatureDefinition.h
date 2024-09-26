#pragma once
#include "BaseStateFeature.h"
#include "InputAction.h"
#include "HSMFeatureDefinition.generated.h"

USTRUCT(BlueprintType)
struct FHSMFeatureDefinition: public FTableRowBase 
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UBaseStateFeature> FeatureType = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UInputAction*> SupportedInputActions = TArray<UInputAction*>{};

	FHSMFeatureDefinition();
};
