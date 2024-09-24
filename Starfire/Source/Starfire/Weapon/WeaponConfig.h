#pragma once
#include "Starfire/Animation/FWeaponAnimData.h"
#include "Starfire/Utility/InputSignalType.h"
#include "WeaponConfig.generated.h"


USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponConfig
{
	GENERATED_BODY()

	//Common
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float Damage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/Project.EInputSignalType"),Category = "Common")
	int32 AllowedInputSignals = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int AmmoCost = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	int MaxClipSize = 12;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Common")
	float FireDelay  =1;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	FTransform DefaultWeaponOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	FTransform AimWeaponOffset;

	//Reloading
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Reloading")
	float ReloatBlendOutTime =0.3f;

	//Melee
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Melee")
	float MeleeDamage = 50.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Melee")
	float MeleeDelay = .5f;
	

	//Other
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Other")
	bool bInfiniteAmmo = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Other")
	TEnumAsByte<ETraceTypeQuery>  TraceTypeQuery = TraceTypeQuery2;

	//Animation
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Animation")
	UWeaponAnimationAsset* WeaponAnimationAsset = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Animation")
	FTransform AimOffset = FTransform();

public:
	FWeaponConfig();
	
	float GetShotAngle(bool bIsAiming);
	float GetAimAccuracy() const;
	float GetAccuracy(bool bIsAiming) const;
	float GetSpread(bool bIsAiming) const;
	float GetAimSpread() const;
	FWeaponAnimData GetAnimData() const;
	
};

