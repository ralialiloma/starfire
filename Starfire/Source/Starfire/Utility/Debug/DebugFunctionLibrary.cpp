// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugFunctionLibrary.h"
#include "DebugSettings.h"

bool UDebugFunctionLibrary::ShouldDebug(FGameplayTag DebugTag, EDebugType DebugType)
{
	return GetDefault<UDebugSettings>() && GetDefault<UDebugSettings>()->ShouldDebug(DebugTag,DebugType);
}