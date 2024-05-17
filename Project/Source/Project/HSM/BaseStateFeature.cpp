// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStateFeature.h"

#include "ECallIInput.h"


void EnterState(){};
void RunFeature(FStateModuleDataStruct Data){};
void ExitState(){};

void UBaseStateFeature::Initialize(UStateCallstack* NewCallstack)
{
	Callstack = NewCallstack;
}

void UBaseStateFeature::RunAction(ECallInput CallInput,FStateModuleDataStruct Data)
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
