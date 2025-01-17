// Fill out your copyright notice in the Description page of Project Settings.

#include "Sf_PlayerCameraManager.h"
#include "Sf_FP_Character.h"
#include "Components/Sf_FP_CharacterMovementComponent.h"
DEFINE_LOG_CATEGORY_STATIC(SF_PlayerCameraManager, Display, Display);

void ASF_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	ASf_FP_Character* SfCharacter = Cast<ASf_FP_Character>(GetOwningPlayerController()->GetPawn());
	if (!IsValid(SfCharacter))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_Character"))
		return;
	}

	USf_FP_CharacterMovementComponent* SfCharacterMovementComponent = SfCharacter->GetSf_FP_CharacterMovement();
	if (!IsValid(SfCharacterMovementComponent))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_CharactermovementComponent"))
		return;
	}
	

	//Update View Target Rotation
	float RollOverwrite =ProcessWallRunRollOverwrite(SfCharacterMovementComponent,SfCharacter,DeltaTime);
	ViewTarget.POV.Rotation = FRotator(ViewTarget.POV.Rotation.Pitch,ViewTarget.POV.Rotation.Yaw,RollOverwrite);

	//Update Aim
	// Get SF_Equipment component.
	USf_Equipment* SFEquipmentComp = SfCharacter->GetSfEquipmentComponent();
	if (!IsValid(SFEquipmentComp))
	{
		UE_LOG(SF_PlayerCameraManager, Warning, TEXT("Invalid SF_EquipmentComponent"));
		return;
	}

	// -- FOV interpolation --
	float CurrentFOV  = GetFOVAngle();
	float TargetFOV   = SFEquipmentComp->IsAiming() ? ADSFieldOfView : DefaultFieldOfView;
	float NewFOV      = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 1.0f / ADSBlendDuration);
	SetFOV(NewFOV);

	// -- Vignette intensity --
	FMinimalViewInfo CacheView = GetCameraCacheView();
	CacheView.PostProcessSettings.bOverride_VignetteIntensity = true;
	float CurrentVignette  = CacheView.PostProcessSettings.VignetteIntensity;
	float TargetVignette   = SFEquipmentComp->IsAiming() ? 1.0f : 0.4f;
	float NewVignette      = FMath::FInterpTo(CurrentVignette, TargetVignette, DeltaTime, 1.0f / ADSBlendDuration);
	CacheView.PostProcessSettings.VignetteIntensity = NewVignette;

	// -- Depth of Field (Blur Effect) --
	CacheView.PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	CacheView.PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	
	// Set DOF values based on aiming state
	if (SFEquipmentComp->IsAiming())
	{
		// Blur for aiming
		CacheView.PostProcessSettings.DepthOfFieldFocalDistance = 100.0f;
		CacheView.PostProcessSettings.DepthOfFieldSensorWidth = 300.0f;
	}
	else
	{
		// Default DOF settings (less blur)
		CacheView.PostProcessSettings.DepthOfFieldFocalDistance = 500.0f;
		CacheView.PostProcessSettings.DepthOfFieldSensorWidth = 0.01f;
	}

	SetCameraCachePOV(CacheView);
}

void ASF_PlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);
}

float ASF_PlayerCameraManager::ProcessWallRunRollOverwrite(USf_FP_CharacterMovementComponent* SfCharacterMovementComponent,
                                                        ASf_FP_Character* SfCharacter, float DeltaTime)
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
