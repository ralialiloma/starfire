#pragma once
#include "CoreMinimal.h"
#include "CF_Death_Config.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_Death.generated.h"


class AResource;

DEFINE_LOG_CATEGORY_STATIC(EF_Death, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UCF_Death : public USf_CharacterFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Death")
	void Kill();

protected:
#pragma endregion
	
#pragma region Properties
public:
	FSf_VoidDelegate_BP OnDeath_BP;
	FSf_VoidDelegate_CPP OnDeath_CPP;
protected:
	UPROPERTY()
	const UCF_Death_Config* Death_Config;
	
	UPROPERTY()
	AWeaponBase* EquippedWeapon;
#pragma endregion


};