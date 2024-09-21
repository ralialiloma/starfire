#pragma once
#include "Project/Utility/FunctionLibrary.h"
#include "WeaponConfig.generated.h"

USTRUCT(Blueprintable)
struct PROJECT_API FWeaponConfig
{
	GENERATED_BODY()

	//Common
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EInputSignalType"),Category = "Common")
	int32 AllowedInputSignals;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int AmmoCost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int MaxClipCost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float FireDelay;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float ReloadTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float Recoil;

	//Accuracy
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	int Range;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float Spread;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float Accuracy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float AccuracyMultiplier;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	int BulletsPerShot;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Accuracy")
	float SpreadMultiplier;

	//Aim
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimFOV;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimAccuracy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float AimSpread;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	bool bUseMultipliers;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim",  meta=(EditCondition="bUseMultipliers", EditConditionHides))
	float AimAccuracyMultiplier;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim",meta=(EditCondition="bUseMultipliers", EditConditionHides))
	float AimSpreadMultiplier;

	//Other
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Other")
	bool bInfiniteAmmo;

	UPROPERTY()
	TEnumAsByte<ETraceTypeQuery>  TraceTypeQuery;

public:
	float GetShotAngle(bool bIsAiming);
	float GetAimAccuracy() const;
	float GetAccuracy(bool bIsAiming) const;
	float GetSpread(bool bIsAiming) const;
	float GetAimSpread() const;
	
};

