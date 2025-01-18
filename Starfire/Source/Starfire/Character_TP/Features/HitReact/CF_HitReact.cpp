// Fill out your copyright notice in the Description page of Project Settings.


#include "CF_HitReact.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"

void UCF_HitReact::Initialize(ASf_TP_Character* OwningCharacterIn, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(OwningCharacterIn, InConfig);

	OwningCharacterIn->GetSfDamageController()->OnDamageReceived_BP.AddDynamic(this, &UCF_HitReact::OnDamage);
}

void UCF_HitReact::OnDamage(float RemainingHealth, float DamageReceived, FVector HitLocation, FVector HitNormal, FGameplayTag DamageType)
{
	LastHitLocation = HitLocation;

	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		LastHitDirection = Character->GetActorLocation() - GetOwningCharacter()->GetActorLocation();
		LastHitDirection.Normalize();
	}
	else
	{
		LastHitDirection = HitNormal;
	}

	if (USkeletalMeshComponent* SkeletalMesh = GetOwningCharacter()->GetMesh())
	{
		LastHitBone = SkeletalMesh->FindClosestBone(HitLocation);
	}
}
