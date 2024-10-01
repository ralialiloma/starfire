#pragma once
#include "BaseStateFeature.h"
#include "InputAction.h"
#include "BaseStateFeatureDefinition.generated.h"

USTRUCT()
struct FBaseStateFeatureDefintion: public FTableRowBase 
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UBaseStateFeature> FeatureType = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UInputAction*> SupportedInputActions = TArray<UInputAction*>{};

	FBaseStateFeatureDefintion();

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};
