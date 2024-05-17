// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateFeature.h"
#include "BaseState.generated.h"

class UStateCallstack;
/**
 * 
 */


//StateData
USTRUCT(BlueprintType)
struct PROJECT_API FStateDefinition: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UBaseState> State;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TSubclassOf<UBaseStateFeature>> OwnedFeatures;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int Priority;
};


UCLASS(Blueprintable)
class PROJECT_API UBaseState : public UBetterObject
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
	
	UPROPERTY()
	TArray<UBaseStateFeature*> Features;

public:

	//Start Up And Utility
	virtual void PostInitProperties() override;
	int GetPriority() const;

	
	//Feature Functons
	TArray<TSubclassOf<UBaseStateFeature>> GetAllOwnedFeatures();

	bool ContainsThisFeature(UBaseStateFeature* Feature);
	
	template<std::derived_from<UBaseStateFeature>FeatureClass>
	UFUNCTION(BlueprintCallable, BlueprintPure,meta = (DeterminesOutputType = "Feature Class"))
	bool TryGetFeature(FeatureClass*& Feature);
	
	bool TryGetFeatureFast(TSubclassOf<UBaseStateFeature> FeatureClass, UBaseStateFeature*& Feature);

	void CreateFeatures(const FSoftObjectPath SoftObjectPath);
	
};
