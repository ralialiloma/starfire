#pragma once
#include "Project/Animation/FWeaponAnimData.h"
#include "WeaponConfig.generated.h"


USTRUCT(Blueprintable)
struct PROJECT_API FWeaponConfig
{
	GENERATED_BODY()

	//Common
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float Damage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EInputSignalType"),Category = "Common")
	int32 AllowedInputSignals = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int AmmoCost = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int MaxClipSize = 12;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float FireDelay  =1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float ReloadTime = 2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float Recoil = 1;

	//Accuracy
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	int Range = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float Spread = 5;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float Accuracy = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float AccuracyMultiplier = 2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	int BulletsPerShot = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float SpreadMultiplier = 1;

	//Aim
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimFOV = 2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimAccuracy = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimSpread = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	bool bUseMultipliers = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim",  meta=(EditCondition="bUseMultipliers", EditConditionHides))
	float AimAccuracyMultiplier = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim",meta=(EditCondition="bUseMultipliers", EditConditionHides))
	float AimSpreadMultiplier = 1;

	//Other
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Other")
	bool bInfiniteAmmo = false;

	UPROPERTY()
	TEnumAsByte<ETraceTypeQuery>  TraceTypeQuery = TraceTypeQuery2;

	//Animation
	UPROPERTY()
	FWeaponAnimData WeaponAnimData = FWeaponAnimData();

public:
	FWeaponConfig();
	
	float GetShotAngle(bool bIsAiming);
	float GetAimAccuracy() const;
	float GetAccuracy(bool bIsAiming) const;
	float GetSpread(bool bIsAiming) const;
	float GetAimSpread() const;
	
};

