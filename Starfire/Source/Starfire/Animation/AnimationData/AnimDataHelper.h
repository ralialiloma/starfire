#pragma once
#include "FP/WeaponAnimData_FP.h"
#include "TP/WeaponAnimData_TP.h"
#include "AnimDataHelper.generated.h"


UCLASS()
class STARFIRE_API UAnimDataHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequence* GetAnimationSequence_TP(
		FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.AnimSequence"))FGameplayTag Tag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_TP(
		FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.Montage"))FGameplayTag AssetType);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_TP(
		FWeaponAnimData_TP AnimData,
		UPARAM(meta=(Categories="Animation.Character.TP.Blendspace"))FGameplayTag AssetType);
	
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimSequenceBase* GetAnimationSequence_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.AnimSequence"))FGameplayTag AssetType);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation",meta = (BlueprintThreadSafe))
	static UAnimMontage* GetAnimationMontage_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.Montage"))FGameplayTag AssetType);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "WeaponAnimation", meta = (BlueprintThreadSafe))
	static UBlendSpace* GetBlendspace_FP(
		FWeaponAnimData_FP AnimData,
		UPARAM(meta=(Categories="Animation.Character.FP.Blendspace"))FGameplayTag AssetType);

	
};
