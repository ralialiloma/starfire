#pragma once
#include "AnimationData/TP/WeaponAnimData_TP.h"
#include "AnimationData/FP/WeaponAnimData_FP.h"
#include "WeaponMontageEventPackage.generated.h"


UENUM(BlueprintType)
enum class EWeaponAnimationEventType : uint8
{
	None = 0 UMETA(Hidden),
	Fire = 1 UMETA(DisplayName = "FirePlayer") ,
	Reload = 2 UMETA(DisplayName = "Reload"),
	Aim = 3 UMETA(DisplayName = "Aim"),
	Equip = 4 UMETA(DisplayName = "Equip"),
	Melee = 5 UMETA(DisplayName = "Melee"),
	Unequip = 6 UMETA(DisplayName = "Unequip"),
};


USTRUCT(BlueprintType)
struct FWeaponMontageEventPackage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EWeaponAnimationEventType AnimationEventType;

	UPROPERTY(BlueprintReadOnly)
	bool bStart;

	UPROPERTY(BlueprintReadOnly)
	FWeaponAnimData_FP AnimData_FP;

	UPROPERTY(BlueprintReadOnly)
	FWeaponAnimData_TP AnimData_TP;

	FWeaponMontageEventPackage()
	   : AnimationEventType(EWeaponAnimationEventType::None), bStart(false), AnimData_FP(FWeaponAnimData_FP()), AnimData_TP(FWeaponAnimData_TP())
	{}
	
	FWeaponMontageEventPackage(
		const EWeaponAnimationEventType InAnimationEventType,
		const bool bInIsStarting,
		const FWeaponAnimData_FP& InAnimData_FP,
		const FWeaponAnimData_TP& InAnimData_TP)
		: AnimationEventType(InAnimationEventType), bStart(bInIsStarting),AnimData_FP(InAnimData_FP), AnimData_TP(InAnimData_TP)
	{}
};