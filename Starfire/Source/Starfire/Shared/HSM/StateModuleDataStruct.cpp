// Fill out your copyright notice in the Description page of Project Settings.


#include "StateModuleDataStruct.h"

FStateModuleDataStruct::FStateModuleDataStruct():
	ActionValue_1(false),
	ActionValue_2(0),
	ActionValue_3(FVector2D::Zero()),
	ElapsedSeconds(0),
	TriggeredSeconds(0),
	InputSignal()
{
}

FStateModuleDataStruct::FStateModuleDataStruct(ACharacter* InCharacter):
	ActionValue_1(false),
	ActionValue_2(0),
	ActionValue_3(FVector2D::Zero()),
	ElapsedSeconds(0),
	TriggeredSeconds(0),
	InputSignal()
{
}
