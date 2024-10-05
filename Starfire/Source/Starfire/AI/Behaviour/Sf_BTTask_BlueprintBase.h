#pragma once
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Sf_BTTask_BlueprintBase.generated.h"

DECLARE_LOG_CATEGORY_CLASS(USf_BTT_BlueprintBase_Log,Log, Log);

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USf_BTTask_BlueprintBase: public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	//UBTTask_BlueprintBase
public:
	USf_BTTask_BlueprintBase(const FObjectInitializer& ObjectInitializer);
	virtual void SetOwner(AActor* NewOwner) override;

	//Interface
public:
	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "USf_BTTask_BlueprintBase")
	ASf_NPCharacter* GetOwningSfNPCharacter() const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "USf_BTTask_BlueprintBase")
	UAnimInstance* GetOwningAnimInstance() const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "USf_BTTask_BlueprintBase")
	AAIController* GetOwningAIController() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);
private:
	UPROPERTY()
	ASf_NPCharacter* OwningNPCharacter;
	
};



