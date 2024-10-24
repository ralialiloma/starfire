#pragma once
#include "Starfire/Animation/Sf_AnimDataController.h"
#include "WeaponAnimMontageController_TP.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Sf_WeaponAnimMontageController_TP, Display, Display);

UCLASS(BlueprintType,ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class USf_WeaponAnimMontageController_TP: public USf_AnimDataController
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual float RunAnimation(FWeaponMontageEventPackage WeaponAnimationUpdateData) const override ;

# pragma region Functions
private:
	float PlayFPMontage(FWeaponAnimData_TP AnimData_FP, FGameplayTag MontageType) const;
	void StopFPMontage(FWeaponAnimData_TP AnimData_FP, FGameplayTag MontageType) const;
# pragma endregion Functions

# pragma region Properties
protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "AnimationData",  meta =(CustomConfig,Categories="Animation.Character.TP.Montage"))
	TMap<EWeaponAnimationEventType,FGameplayTag> AnimationStartMappings;
# pragma endregion Properties

};