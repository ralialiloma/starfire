// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateFeature.h"
#include "BaseState.generated.h"

class UStateCallstack;
/**
 * 
 */


UCLASS(Blueprintable)
class PROJECT_API UBaseState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UStateCallstack* StateCallstack = nullptr;
	
protected:

	UPROPERTY(BlueprintReadWrite)
	int Priority = 0;
	

	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UBaseStateFeature>> OwnedFeatures;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseStateFeature*> Features;

public:
	//void EnterState();
	//void ExitState();
	int GetPriority() const;
	
	template<std::derived_from<UBaseStateFeature>FeatureClass>
	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (DeterminesOutputType = "Feature Class"))
	bool TryGetFeature(FeatureClass*& Feature);
	
	bool TryGetFeatureFast(TSubclassOf<UBaseStateFeature> FeatureClass, UBaseStateFeature*& Feature);
	void CreateFeatures();

	virtual void PostInitProperties() override;
		
    

	
};
