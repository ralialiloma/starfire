// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include  "ECallIInput.h"
#include "StateModuleDataStruct.h"
#include "InputAction.h"
#include "Starfire/Sf_Bases/Sf_Object.h"
#include "BaseStateFeature.generated.h"

class UBaseState;
class UStateCallstack;
class ASf_FP_Character;

UCLASS(Abstract, Blueprintable)
class STARFIRE_API UBaseStateFeature : public USf_Object
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UStateCallstack* Callstack;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<UInputAction*> SupportedInputActions;
private:
	UPROPERTY()
	ASf_FP_Character* OwningCharacter;
	
public:

	void Initialize(UStateCallstack* NewCallstack);

	UFUNCTION(BlueprintImplementableEvent)
	void EnterState();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivate();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RunFeature(FStateModuleDataStruct Data);

	UFUNCTION(BlueprintImplementableEvent)
	void ExitState();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeactivate();

	void RunAction(ECallInput CallInput, const FStateModuleDataStruct& Data);
	
	virtual void GetSupportedInputActions(TArray<UInputAction*> &OutSupportedInputActions);

	UFUNCTION(BlueprintCallable)
	 ASf_FP_Character* GetOwningCharacter() const;

	UFUNCTION(BlueprintCallable)
	AController* GetOwningPlayerController() const;

private:
	void ImportFeatureDefinition(FSoftObjectPath BaseStateFeatureDefDT);


	
};
