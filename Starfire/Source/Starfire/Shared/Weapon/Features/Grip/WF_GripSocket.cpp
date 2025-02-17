#include "WF_GripSocket.h"
#include "WF_GripSocket_Config.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_GripSocket::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_GripSocket_Config,Config);
}

FTransform UWF_GripSocket::GetGripTransform()
{
	if (!IsValid(Config))
		return FTransform::Identity;

	return  GetOwningWeapon()->GetSkeletalMesh()->GetSocketTransform(Config->SocketName,ERelativeTransformSpace::RTS_World);
}


