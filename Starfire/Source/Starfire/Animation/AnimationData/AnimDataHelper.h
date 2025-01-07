#pragma once
#include "FP/WeaponAnimData_FP.h"
#include "TP/WeaponAnimData_TP.h"
#include "Weapon/WeaponAnimData_Weapon.h"
#include "AnimDataHelper.generated.h"


UCLASS()
class STARFIRE_API UAnimDataHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequence* GetAnimationSequence_TP(
		const FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.AnimSequence"))const FGameplayTag Tag, bool& bIsValid);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequence* GetSafeAnimationSequence_TP(
		const FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.AnimSequence"))const FGameplayTag Tag, UAnimSequence* BackupSequence);

	UFUNCTION(BlueprintCallable,Category = "WeaponAnimation")
	void ReplaceEmptyWithDefaultsTP(FWeaponAnimData_TP AnimData, UAnimSequence* DefautAnimSequene, UBlendSpace* DefaultBlendSpace);
	UFUNCTION(BlueprintCallable,Category = "WeaponAnimation")
	static  void ReplaceEmptyWithDefaults(UPARAM(ref)FWeaponAnimData_TP& AnimData, UAnimSequence* DefaultAnimSequence, UBlendSpace* DefaultBlendSpace);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_TP(
		const FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.Montage"))const FGameplayTag AssetType, bool& bIsValid);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_TP(
		FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.Blendspace"))FGameplayTag AssetType, bool& bIsValid);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UBlendSpace* GetSafeBlendspace_TP(
		const FWeaponAnimData_TP& AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.Blendspace"))const FGameplayTag AssetType, UBlendSpace* BackupBlendSpace);
	
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequenceBase* GetAnimationSequence_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.AnimSequence"))FGameplayTag AssetType);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.Montage"))FGameplayTag AssetType);
	
	 static UAnimMontage* GetAnimationMontage_Weapon(FWeaponAnimData_Weapon AnimData,
	 	UPARAM(meta=(Categories="Animation.Weapon.Montage"))FGameplayTag AssetType);

	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation", meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.Blendspace"))FGameplayTag AssetType);

	
};
