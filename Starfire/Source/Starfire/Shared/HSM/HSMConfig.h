#pragma once
#include "BaseState.h"
#include "Starfire/Character_FP/Components/Sf_FP_CharacterMovementComponent.h"
#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "HSMConfig.generated.h"


USTRUCT(BlueprintType)
struct FHSMConfig
{
	GENERATED_BODY()

public:

	FHSMConfig();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Equipment")
	TMap<TEnumAsByte<EEquipmentFlags>,TSubclassOf<UBaseState>> EquipmentFlagMap;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Movement")
	TMap<TEnumAsByte<EMovementMode>,TSubclassOf<UBaseState>> MovementModeFlag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Movement")
	TMap<TEnumAsByte<ECustomMovementMode>,TSubclassOf<UBaseState>> CustomMovementModeFlag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Movement")
	TSubclassOf<UBaseState> SprintFlag;
};


UCLASS(BlueprintType)
class UHSMConfiguration: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FHSMConfig ConfigStruct;

public:
	UFUNCTION(CallInEditor, Category = "HSMConfiguration")
	void Refresh();

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};



