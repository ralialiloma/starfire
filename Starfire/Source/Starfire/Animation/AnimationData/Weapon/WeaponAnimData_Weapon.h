#pragma once
#include "GameplayTagContainer.h"
#include "WeaponAnimData_Weapon.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponAnimData_Weapon:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData_Weapon();
	
	void UpdateEntries();

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<FGameplayTag, UAnimMontage*> AnimationMontages = TMap<FGameplayTag, UAnimMontage*>();
};


UCLASS(Blueprintable)
class STARFIRE_API UWeaponAnimationAsset_Weapon:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset_Weapon();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData_Weapon WeaponAnimData;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(CallInEditor)
	void Refresh();
#endif
	
};

