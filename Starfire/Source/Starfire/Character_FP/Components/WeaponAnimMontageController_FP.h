#pragma once
#include "Starfire/Animation/WeaponAnimMontageController.h"
#include "Starfire/Animation/AnimationData/FP/WeaponAnimationMontageType_FP.h"
#include "WeaponAnimMontageController_FP.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Sf_WeaponAnimMontageController_FP, Display, Display);

UCLASS(BlueprintType,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class USf_WeaponAnimMontageController_FP: public USf_WeaponAnimMontageController
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual float RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const override ;

# pragma region Functions
private:
	float PlayFPMontage(FWeaponAnimData_FP AnimData_FP, EWeaponAnimationMontageType_FP MontageType) const;
	void StopFPMontage(FWeaponAnimData_FP AnimData_FP, EWeaponAnimationMontageType_FP MontageType) const;
# pragma endregion Functions

# pragma region Properties
protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "AnimationData",  meta =(CustomConfig))
	TMap<EWeaponAnimationEventType,EWeaponAnimationMontageType_FP> AnimationStartMappings;
# pragma endregion Properties

};
