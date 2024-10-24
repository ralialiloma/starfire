#pragma once
#include "GameplayTagContainer.h"
#include "WeaponAnimData_TP.generated.h"

USTRUCT(Blueprintable)
struct STARFIRE_API FWeaponAnimData_TP:public FTableRowBase
{
	GENERATED_BODY()
	
	FWeaponAnimData_TP();

	void UpdateEntries();
	
	TMap<FGameplayTag ,UAnimSequence*> GetAllSequences() const;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag ,UAnimSequence*> AnimationSequences_UB;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag ,UAnimSequence*> AnimationSequences_LB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow), Category = "Sequences")
	TMap<FGameplayTag ,UAnimSequence*> AnimationSequences_FB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<FGameplayTag, UAnimMontage*> AnimationMontages;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(ReadOnlyKeys,ForceInlineRow))
	TMap<FGameplayTag, UBlendSpace*> Blendspaces;
};


UCLASS(Blueprintable)
class STARFIRE_API UWeaponAnimationAsset_TP:public UDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationAsset_TP();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponAnimData_TP WeaponAnimData;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
};

