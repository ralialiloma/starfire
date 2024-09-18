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
	UPROPERTY(EditDefaultsOnly) float WallRunBlendDuration = .2f;
	UPROPERTY(EditDefaultsOnly) float WallRunCameraRollAngle = 11.5f;
private:
	float WallRunBlendTime = 0.f;
	float WallRunBlendSmoothing = 10.f;
	float SmoothedTargetRoll = 0.f;
	
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

private:
	float ProcessWallRunRollOverwrite(USF_CharacterMovementComponent* SfCharacterMovementComponent,
	                               ASf_Character* SfCharacter, float DeltaTime);

};
