#pragma once
#include "Starfire/Animation/Sf_AnimDataController.h"
#include "Starfire/Animation/AnimationData/FP/WeaponAnimationMontageType_FP.h"
#include "AnimDataController_FP.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Sf_WeaponAnimMontageController_FP, Display, Display);

UCLASS(BlueprintType,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class USf_AnimDataController_FP: public USf_AnimDataController
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual float RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const override ;

# pragma region Functions
private:
	float PlayFPMontage(
		const FWeaponAnimData_FP& AnimData_FP,
		UPARAM(meta=(Categories="Animation.Character.FP.Montage"))FGameplayTag MontageType) const;
	
	void StopFPMontage(
		FWeaponAnimData_FP AnimData_FP,
		UPARAM(meta=(Categories="Animation.Character.FP.Montage"))FGameplayTag MontageType) const;
# pragma endregion Functions

# pragma region Properties
protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "AnimationData",  meta =(CustomConfig,Categories="Animation.Character.FP.Montage"))
	TMap<EWeaponAnimationEventType,FGameplayTag> AnimationMontageMappings;
# pragma endregion Properties

};
