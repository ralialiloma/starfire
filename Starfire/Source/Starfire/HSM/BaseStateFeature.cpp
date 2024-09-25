// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStateFeature.h"

#include "ECallIInput.h"
#include "Kismet/KismetStringLibrary.h"
#include "Starfire/Utility/DebugSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(SF_BaseStateFeature, Log, All);

void UBaseStateFeature::Initialize(UStateCallstack* NewCallstack)
{
	Callstack = NewCallstack;
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
	
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECallInput"), true);
	if (!EnumPtr)
	{
		UE_LOG(SF_BaseStateFeature, Warning, TEXT("Enum not found for CallInput"));
		return;
	}

	if (UDebugSubsystem::GetHSMDebug(EDebugType::Log))
	{
		FString CallInputString = EnumPtr->GetNameStringByValue(static_cast<int32>(CallInput));
		UE_LOG(SF_BaseStateFeature, Log, TEXT("%s Feature: %s"), *CallInputString, *GetClass()->GetName());
	}
	
}
