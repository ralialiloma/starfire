// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"

EInputSignalType UFunctionLibrary::ConvertToInputSignalType(EInputSignalType SignalType)
{
	return  SignalType;
}

int UFunctionLibrary::ConvertEnumToInteger(uint8 Byte)
{
	return FMath::Pow(2,static_cast<float>(Byte));
}

UObject* UFunctionLibrary::GetAsType(TSubclassOf<UObject> OutputClass, UObject* ObjectToCovert)
{
	{
		if (!ObjectToCovert)
		{
			return nullptr;
		}
		if (ObjectToCovert->GetClass()->IsChildOf(OutputClass))
		{
			return ObjectToCovert;
		}
		return nullptr;
	}
}

UObject* UFunctionLibrary::Cast(TEnumAsByte<ESuccessState>& Success, TSubclassOf<UObject> OutputClass,
	UObject* ObjectToCovert)
{
	{
		if (!ObjectToCovert)
		{
			Success = ESuccessState::Failed;
			return nullptr;
		}
		if (ObjectToCovert->GetClass()->IsChildOf(OutputClass))
		{
			Success = ESuccessState::Success;
			return ObjectToCovert;
		}
		Success = ESuccessState::Failed;
		return nullptr;
	}
}
