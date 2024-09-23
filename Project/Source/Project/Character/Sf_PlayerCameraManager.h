// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Movement/SF_CharacterMovementComponent.h"
#include "Sf_PlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ASF_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()


//Wall Run Cam
public:
	UPROPERTY(EditDefaultsOnly, Category = "WallRun") float WallRunBlendDuration = .2f;
	UPROPERTY(EditDefaultsOnly, Category = "WallRun") float WallRunCameraRollAngle = 11.5f;

private:
	float WallRunBlendTime = 0.f;
	float WallRunBlendSmoothing = 10.f;
	float SmoothedTargetRoll = 0.f;

//AimDownSight
	UPROPERTY(EditDefaultsOnly, Category = "AimDownSight") float DefaultFieldOfView = 90.f; 
	UPROPERTY(EditDefaultsOnly, Category = "AimDownSight") float ADSFieldOfView = 40.f; 
	UPROPERTY(EditDefaultsOnly, Category = "AimDownSight") float DefaultVignetteIntensity = .5f ;
	UPROPERTY(EditDefaultsOnly, Category = "AimDownSight") float ADSVignette = 1;
	UPROPERTY(EditDefaultsOnly, Category = "AimDownSight") float ADSBlendDuration = .2f;
	
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

private:
	float ProcessWallRunRollOverwrite(USF_CharacterMovementComponent* SfCharacterMovementComponent,
	                               ASf_Character* SfCharacter, float DeltaTime);

};
