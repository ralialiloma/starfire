// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateCallstack.h"
#include "SF_CharacterStateMachine.generated.h"

class ASf_Character;

DECLARE_DELEGATE_OneParam(FRunCallStackByInputAction, const UInputAction);

UCLASS(Blueprintable, ClassGroup=(StateMachine), meta=(BlueprintSpawnableComponent) )
class STARFIRE_API USf_CharacterStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USf_CharacterStateMachine(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool TryAddState(TSubclassOf<UBaseState> BaseStateClass);
	
	UFUNCTION(BlueprintCallable)
	bool TryRemoveState(TSubclassOf<UBaseState> BaseStateClass);

	UFUNCTION(BlueprintCallable)
	void SubscribeToInputActions(TArray<UInputAction*> InputActions);

	template<EInputSignalType InputSignalType>
	void RunPressedAction(const FInputActionValue& InputActionValue);

private:
	void RunCallStackByInputAction(UInputAction InputAction);

	
protected:

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SF_CharacterStateMachine")
	UStateCallstack* StateCallstack;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "SF_CharacterStateMachine",Meta=(RequiredAssetDataTags = "RowStructure=/Script/Starfire.StateDefinition"))
	UDataTable* StateDefinitionDT;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "SF_CharacterStateMachine",Meta=(RequiredAssetDataTags = "RowStructure=/Script/Starfire.HSMFeatureDefinition"))
	UDataTable* StateFeatureDefinitionDT;

	UPROPERTY(BlueprintReadOnly)
	ASf_Character* OwningCharacter;
};

