// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterObject.h"

void UBetterObject::PostInitProperties()
{
    UObject::PostInitProperties();
    if (GetWorld())
    {
        OnConstruct();
    }
}

UWorld* UBetterObject::GetWorld() const
{
    if (GetOuter() == nullptr)
    {
        return nullptr;
    }
		
    if (Cast<UPackage>(GetOuter()) != nullptr)
    {
        return Cast<UWorld>(GetOuter()->GetOuter());
    }
		
    return GetOwningActor()->GetWorld();
}

AActor* UBetterObject::GetOwningActor() const
{
    return GetTypedOuter<AActor>();
}

void UBetterObject::DestroyObject()
{
    if (IsValid(this))
    {
        OnDestroyed();
        MarkAsGarbage();
    }
}