#pragma once
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Starfire/AI/Sf_NPCharacter.h"
#include "Sf_BTService_BlueprintBase.generated.h"

DECLARE_LOG_CATEGORY_CLASS(Sf_BTService_BlueprintBase_Log,Log, Log);

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

private:
	UPROPERTY()
	ASf_NPCharacter* OwningNPCharacter;
	
};
