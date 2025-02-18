// Fill out your copyright notice in the Description page of Project Settings.


#include "CF_HitReact.h"

#include "CF_HitReact_Config.h"
#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"

void UCF_HitReact::Initialize(ASf_TP_Character* OwningCharacterIn, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(OwningCharacterIn, InConfig);
	VALIDATE_CONFIG(UCF_HitReact_Config,HitReactConfig)
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

	//Calculate Montage
	UAnimMontage* Montage = nullptr;
	
	FVector HitDirection = LastHitDirection.GetSafeNormal();
	FVector Forward = GetOwningCharacter()->GetActorForwardVector();
	FVector Right = GetOwningCharacter()->GetActorRightVector();
	
	float ForwardDot = HitDirection.Dot(Forward);
	float RightDot = HitDirection.Dot(Right);
	if (ForwardDot > 0.5f && HitReactConfig->FrontHits.Num() > 0) //Frontal Hit
	{
		Montage = HitReactConfig->FrontHits[FMath::RandRange(0, HitReactConfig->FrontHits.Num() - 1)];
	}
	else if (ForwardDot < -0.5f && HitReactConfig->BackHits.Num() > 0) //Back Hit
	{
		Montage = HitReactConfig->BackHits[FMath::RandRange(0, HitReactConfig->BackHits.Num() - 1)];
	}
	else if (RightDot > 0.0f && HitReactConfig->RightHits.Num() > 0) //Right Side Hit
	{
		Montage = HitReactConfig->RightHits[FMath::RandRange(0, HitReactConfig->RightHits.Num() - 1)];
	}
	else if (RightDot < 0.0f && HitReactConfig->LeftHits.Num() > 0) //Left Side Hit
	{
		Montage = HitReactConfig->LeftHits[FMath::RandRange(0, HitReactConfig->LeftHits.Num() - 1)];
	}
	
	PlayMontage(Montage);
}

void UCF_HitReact::PlayMontage(UAnimMontage* Montage)
{
	if (!Montage) 
		return;

	ACharacter* OwnerCharacter = GetOwningCharacter();
	if (!OwnerCharacter) 
		return;

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) 
		return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) 
		return;

	if (!bMontageCooldown)
	{
		bMontageCooldown = true;
		AnimInstance->Montage_Play(Montage);

		// Bind to Montage Blend Out
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &UCF_HitReact::OnMontageBlendingOut);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, Montage);
	}
}

void UCF_HitReact::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	bMontageCooldown = false;
}
