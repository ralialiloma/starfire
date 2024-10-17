#pragma once
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Sf_BTService_BlueprintBase.generated.h"

DECLARE_LOG_CATEGORY_CLASS(Sf_BTService_BlueprintBase,Log, Log);

UCLASS(Abstract, Blueprintable, MinimalAPI)
class USf_BTService_BlueprintBase: public UBTService_BlueprintBase
{
	GENERATED_BODY()

	//UBTService_BlueprintBase
public:
	virtual void SetOwner(AActor* NewOwner) override;
	USf_BTService_BlueprintBase(const FObjectInitializer& ObjectInitializer);

	//Interface
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sf_BTService_BlueprintBase")
	ASf_NPCharacter* GetOwningNPCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sf_BTService_BlueprintBase")
	FVector GetOwningNPCharacterLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sf_BTService_BlueprintBase")
	UBlackboardComponent* GetOwningBlackboardComponent() const;

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
	ASf_NPCharacter* OwningAIHolder;
	
};
