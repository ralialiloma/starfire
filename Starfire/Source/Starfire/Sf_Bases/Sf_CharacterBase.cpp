// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_CharacterBase.h"

void ASf_CharacterBase::PawnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GetDebugName(this) + " has been killed!");
	OnPawnDeath.Broadcast();
}