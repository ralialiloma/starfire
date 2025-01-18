#include "WF_CameraShake.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_CameraShake::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	PlayerCameraManager =  UGameplayStatics::GetPlayerCameraManager(this,0);
	VALIDATE_WF_CONFIG(UWF_CameraShake_Config,Config);
}



void UWF_CameraShake::OnFire_Implementation()
{
	if (PlayerCameraManager)
		PlayerCameraManager->StartCameraShake(Config->ShakeClass,Config->Scale,Config->PlaySpace);
}



