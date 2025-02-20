#include "WF_ProceduralRecoil.h"

#include "Kismet/KismetMathLibrary.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


void UWF_ProceduralRecoil::OnInit_Implementation()
{
	Super::OnInit_Implementation();
	VALIDATE_WF_CONFIG(UWF_ProceduralRecoil_Config,Config);
}

void UWF_ProceduralRecoil::OnFire_Implementation()
{
	Super::OnFire_Implementation();
	ActivateRecoil();
}

void UWF_ProceduralRecoil::OnTick_Implementation(float OnTick)
{
	Super::OnTick_Implementation(OnTick);
	CurrentRecoil = UKismetMathLibrary::TInterpTo(CurrentRecoil, RecoilTarget, GetWorld()->GetDeltaSeconds(), 25.0f);
	RecoilTarget = UKismetMathLibrary::TInterpTo(RecoilTarget, FTransform::Identity, GetWorld()->GetDeltaSeconds(), 16.0f);
}

void UWF_ProceduralRecoil::ActivateRecoil()
{
	if (!IsValid(Config))
		return;

	float X =  UKismetMathLibrary::RandomFloatInRange(Config->RecoilLocationRangeMin.X, Config->RecoilLocationRangeMax.X);
	float Y =  UKismetMathLibrary::RandomFloatInRange(Config->RecoilLocationRangeMin.Y, Config->RecoilLocationRangeMax.Y);
	float Z=  UKismetMathLibrary::RandomFloatInRange(Config->RecoilLocationRangeMin.Z, Config->RecoilLocationRangeMax.Z);

	float Pitch =  UKismetMathLibrary::RandomFloatInRange(Config->RecoilRotationStrengthMin.Pitch, Config->RecoilRotationStrengthMax.Pitch);
	float Yaw =  UKismetMathLibrary::RandomFloatInRange(Config->RecoilRotationStrengthMin.Yaw, Config->RecoilRotationStrengthMax.Yaw);
	float Roll =  UKismetMathLibrary::RandomFloatInRange(Config->RecoilRotationStrengthMin.Roll, Config->RecoilRotationStrengthMax.Roll);

	ActiveRecoilLocation = FVector(X,Y,Z)*Config->Multiplier;
	ActiveRecoilRotation = FRotator(Pitch,Yaw,Roll)*Config->Multiplier;

	RecoilTarget = FTransform(ActiveRecoilRotation, ActiveRecoilLocation);
}

FVector UWF_ProceduralRecoil::GetActiveRecoilLocation() const
{
	return ActiveRecoilLocation; 
}

FRotator UWF_ProceduralRecoil::GetActiveRecoilRotation() const
{
	return ActiveRecoilRotation;
}

FTransform UWF_ProceduralRecoil::GetCurrentRecoil() const
{
	return CurrentRecoil;
}

