// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Object.h"

void USf_Object::PostInitProperties()
{
    UObject::PostInitProperties();
    if (GetWorld())
    {
        OnConstruct();
    }
}

UWorld* USf_Object::GetWorld() const
{
    if (GetOuter() == nullptr)
    {
        return nullptr;
    }
		
    if (Cast<UPackage>(GetOuter()) != nullptr)
    {
        return Cast<UWorld>(GetOuter()->GetOuter());
    }

    if (GetOwningActor() == nullptr)
    {
        return nullptr;
    }
		
    return GetOwningActor()->GetWorld();
}

AActor* USf_Object::GetOwningActor() const
{
    return GetTypedOuter<AActor>();
}

void USf_Object::DestroyObject()
{
    if (IsValid(this))
    {
        OnDestroyed();
        MarkAsGarbage();
    }
}