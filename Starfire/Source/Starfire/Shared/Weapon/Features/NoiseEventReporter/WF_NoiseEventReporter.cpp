#include "WF_NoiseEventReporter.h"
#include "Perception/AISense_Hearing.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_NoiseEventReporter::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_NoiseEventReporter_Config,Config);
}



void UWF_NoiseEventReporter::OnFire_Implementation()
{
	UAISense_Hearing::ReportNoiseEvent(this,GetOwningWeapon()->GetActorLocation(),Config->FireVolume,GetOwningWeapon()->GetWeaponOwner());	
}



