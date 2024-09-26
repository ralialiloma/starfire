// Fill out your copyright notice in the Description page of Project Settings.


#include "StateModuleDataStruct.h"

FStateModuleDataStruct::FStateModuleDataStruct():
	ActionValue_1(false),
	ActionValue_2(0),
	ElapsedSeconds(0),
	TriggeredSeconds(0),
	InputSignal()
{
}

FStateModuleDataStruct::FStateModuleDataStruct(
	bool InActionValue1,
	int32 InActionValue2,
	FVector2d InActionValue3,
	float InElapsedSeconds,
	float InTriggeredSeconds,
	const EInputSignalType InInputSignal)
	: ActionValue_1(InActionValue1),
	  ActionValue_2(InActionValue2),
		ActionValue_3(InActionValue3),
	  ElapsedSeconds(InElapsedSeconds),
	  TriggeredSeconds(InTriggeredSeconds),
	  InputSignal(InInputSignal)

{
}
