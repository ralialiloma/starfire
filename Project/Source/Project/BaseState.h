// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateFeature.h"
#include "UObject/NoExportTypes.h"
#include "BaseState.generated.h"

/**
 * 
 */


UCLASS()
class PROJECT_API UBaseState : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite)
	int Priority = 0;

	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseStateFeature*> BaseStateFeatures;

public:
	void EnterState();
	void ExitState();
	int GetPriority();

	template<std::derived_from<UBaseStateFeature>FeatureClass>
	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (DeterminesOutputType = "Feature Class"))
	bool TryGetFeature(FeatureClass*& Feature);
	bool TryGetFeatureFast(TSubclassOf<UBaseStateFeature> FeatureClass, UBaseStateFeature*& Feature);
};
