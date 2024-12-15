// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStateFeature.h"

#include "BaseStateFeatureDefinition.h"
#include "ECallIInput.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(SF_BaseStateFeature, Log, All);

void UBaseStateFeature::Initialize(UStateCallstack* NewCallstack)
{
	Callstack = NewCallstack;
	OwningCharacter = Cast<ASf_FP_Character>(Callstack->OwningCharacter) ;

	if (!IsValid(OwningCharacter))
	{
		UE_LOG(SF_BaseStateFeature, Error, TEXT("Invalid Owning Character"));
	}
}

void UBaseStateFeature::RunAction(ECallInput CallInput, const FStateModuleDataStruct& Data)
{
	switch (CallInput)
	{
		case Enter:
			EnterState();
			break;
		case Run:
			RunFeature(Data);
			break;
		case Exit:
			ExitState();
			break;
	default:
		UE_LOG(SF_BaseStateFeature, Warning, TEXT("Unhandled CallInput: %d"), static_cast<int32>(CallInput));
		return;
	}

	if (!this)
	{
		UE_LOG(SF_BaseStateFeature, Error, TEXT("Invalid instance for logging"));
		return;
	}
	

	if (USF_DebugFunctionLibrary::ShouldDebug(Sf_GameplayTags::Debug::FP::HSM::Name,EDebugType::Log))
	{
		FString CallInputString = USf_FunctionLibrary::GetEnumAsString<ECallInput>(CallInput);
		UE_LOG(SF_BaseStateFeature, Log, TEXT("%s Feature: %s"), *CallInputString, *GetClass()->GetName());
	}
	
}

void UBaseStateFeature::GetSupportedInputActions(TArray<UInputAction*> &OutSupportedInputActions)
{
	OutSupportedInputActions.Append(SupportedInputActions);
}

ASf_FP_Character* UBaseStateFeature::GetOwningCharacter() const
{
	return OwningCharacter;
}

AController* UBaseStateFeature::GetOwningPlayerController() const
{
	return OwningCharacter->GetController();
}

void UBaseStateFeature::ImportFeatureDefinition(FSoftObjectPath BaseStateFeatureDefDT)
{
	TArray<FBaseStateFeatureDefintion> FeatureDefintions = 
		USf_FunctionLibrary::GetRowDataFromDT<FBaseStateFeatureDefintion>(BaseStateFeatureDefDT);

	for (FBaseStateFeatureDefintion Defintion: FeatureDefintions)
	{
		if (Defintion.FeatureType != GetClass())
			continue;

		SupportedInputActions =  Defintion.SupportedInputActions;
	}
}
