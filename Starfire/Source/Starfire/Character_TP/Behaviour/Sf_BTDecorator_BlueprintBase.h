#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Sf_BTDecorator_BlueprintBase.generated.h"

DECLARE_LOG_CATEGORY_CLASS(USf_BTDecorator_BlueprintBase_Log,Log, Log);

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USf_BTDecorator_BlueprintBase: public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()

public:
	USf_BTDecorator_BlueprintBase(const FObjectInitializer& ObjectInitializer);
	virtual void SetOwner(AActor* NewOwner) override;

	//Interface
public:
	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "Sf_BTDecorator_BlueprintBase")
	ASf_TP_Character* GetOwningSfNPCharacter() const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "Sf_BTDecorator_BlueprintBase")
	UAnimInstance* GetOwningAnimInstance() const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "Sf_BTDecorator_BlueprintBase")
	AAIController* GetOwningAIController() const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category =  "Sf_BTDecorator_BlueprintBase")
	UBlackboardComponent* GetOwningBlackboard() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sf_BTDecorator_BlueprintBase", meta = (DeterminesOutputType = "Class"))
	USf_CharacterFeature* GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class);

	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void Sf_SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void Sf_SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void Sf_SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void Sf_SetBlackboardVectorValue(ELocationBlackboardKey ActorBlackboardKey, FVector Value);
private:
	UPROPERTY()
	ASf_TP_Character* OwningNPCharacter;
	
};



