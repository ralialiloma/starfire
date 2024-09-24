// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStateFeature.h"

#include "ECallIInput.h"

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
	}
}
