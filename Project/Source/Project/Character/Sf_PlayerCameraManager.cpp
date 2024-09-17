// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_PlayerCameraManager.h"

#include "SF_Character.h"
#include "Movement/SF_CharacterMovementComponent.h"

void ASF_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ASf_Character* SfCharacter = Cast<ASf_Character>(GetOwningPlayerController()->GetPawn()))
	{
		USF_CharacterMovementComponent* SfCharacterMovementComponent = SfCharacter->GetSfCharacterMovementComponent();
		
		if (SfCharacterMovementComponent!=nullptr)
		{
			float RollOffset =ProcessWallRunRollOffset(SfCharacterMovementComponent,SfCharacter,DeltaTime);
			ViewTarget.POV.Rotation = FRotator(ViewTarget.POV.Rotation.Pitch,ViewTarget.POV.Rotation.Yaw,RollOffset);
		}
			
	}
}

void ASF_PlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);
}

float ASF_PlayerCameraManager::ProcessWallRunRollOffset(USF_CharacterMovementComponent* SfCharacterMovementComponent,
                                                        ASf_Character* SfCharacter, float DeltaTime)
{
	float RollOffset = 0;
	const bool bIsWallRunning = SfCharacterMovementComponent->IsWallRunning();
	
	if (bIsWallRunning)
	{
		const bool bWallRunningR =  SfCharacterMovementComponent->WallRunningIsRight();
		RollOffset = WallRunCameraRollAngle* (bWallRunningR?-1:1);
	}
	
	const float TargetRoll = RollOffset;
	const float BlendAlpha = FMath::Clamp(WallRunBlendTime/WallRunBlendDuration,0.f,1.f);
	float RotationOffset = FMath::Lerp(0,TargetRoll,BlendAlpha);
		
	if (bIsWallRunning)
		WallRunBlendTime = FMath::Clamp(WallRunBlendTime+DeltaTime,0.f,WallRunBlendDuration);
	else
		WallRunBlendTime = FMath::Clamp(WallRunBlendTime-DeltaTime,0.f,WallRunBlendDuration);

	return  RotationOffset;
}
