// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PlayerCameraManager.h"
#include "Sf_Character.h"
#include "Movement/Sf_FP_CharacterMovementComponent.h"
#include "Sf_Equipment.h"
DEFINE_LOG_CATEGORY_STATIC(SF_PlayerCameraManager, Display, Display);

void ASF_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	ASf_Character* SfCharacter = Cast<ASf_Character>(GetOwningPlayerController()->GetPawn());
	if (!IsValid(SfCharacter))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_Character"))
		return;
	}

	USf_FP_CharacterMovementComponent* SfCharacterMovementComponent = SfCharacter->GetSfCharacterMovementComponent();
	if (!IsValid(SfCharacterMovementComponent))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_CharactermovementComponent"))
		return;
	}

	USF_Equipment* SFEquipmentComp = SfCharacter->GetSFEquipmentComponent();
	if (!IsValid(SfCharacterMovementComponent))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_CharactermovementComponent"))
		return;
	}
	

	//Update View Target Rotation
	float RollOverwrite =ProcessWallRunRollOverwrite(SfCharacterMovementComponent,SfCharacter,DeltaTime);
	ViewTarget.POV.Rotation = FRotator(ViewTarget.POV.Rotation.Pitch,ViewTarget.POV.Rotation.Yaw,RollOverwrite);

	//Update Aim

	//FOV
	float CurrentFOV = GetFOVAngle();
	float TargetFOV = SFEquipmentComp->IsAiming()?ADSFieldOfView:DefaultFieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV,TargetFOV,DeltaTime,1.0f/ADSBlendDuration);
	SetFOV(NewFOV);

	//Vignette
	//float CurrentVignette = GetFOVAngle();
	//float TargetVignette = SFEquipmentComp->IsAiming()?DefaultVignetteIntensity:ADSVignette;
	//float NewVignette = FMath::FInterpTo(CurrentFOV,TargetFOV,DeltaTime,1.0f/ADSBlendDuration);
	
	
}

void ASF_PlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);
}

float ASF_PlayerCameraManager::ProcessWallRunRollOverwrite(USf_FP_CharacterMovementComponent* SfCharacterMovementComponent,
                                                        ASf_Character* SfCharacter, float DeltaTime)
{
	const bool bIsWallRunning = SfCharacterMovementComponent->IsWallRunning();
	
	//Find TargetOffset
	float TargetRoll = 0;
	if (bIsWallRunning)
	{
		const bool bWallRunningR =  SfCharacterMovementComponent->WallRunningIsRight();
		TargetRoll = WallRunCameraRollAngle* (bWallRunningR?-1:1);
	}

	//Update WallRunBlendTime
	if (bIsWallRunning)
		WallRunBlendTime = FMath::Clamp(WallRunBlendTime+DeltaTime,0.f,WallRunBlendDuration);
	else
		WallRunBlendTime = FMath::Clamp(WallRunBlendTime-DeltaTime,0.f,WallRunBlendDuration);

	const float BlendAlpha = FMath::Clamp(WallRunBlendTime/WallRunBlendDuration,0.f,1.f);

	float CurrentRoll = ViewTarget.POV.Rotation.Roll;
	SmoothedTargetRoll = FMath::FInterpTo(SmoothedTargetRoll, TargetRoll, DeltaTime, WallRunBlendSmoothing);
	float RollOverwrite = FMath::Lerp (CurrentRoll, SmoothedTargetRoll, BlendAlpha);

	return  RollOverwrite;
}
