// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateCallstack.h"
#include "SF_CharacterStateMachine.generated.h"

class ASf_Character;


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
	
protected:

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SF_CharacterStateMachine")
	UStateCallstack* StateCallstack;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "SF_CharacterStateMachine",Meta=(RequiredAssetDataTags = "RowStructure=/Script/Starfire.StateDefinition"))
	UDataTable* StateDefinitionDT;
	
	UPROPERTY(BlueprintReadOnly)
	ASf_Character* OwningCharacter;
};
