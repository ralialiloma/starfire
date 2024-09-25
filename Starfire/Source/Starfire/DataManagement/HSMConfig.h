#pragma once
#include "Starfire/Character/Sf_Equipment.h"
#include "Starfire/HSM/BaseState.h"
#include "HSMConfig.generated.h"


USTRUCT(BlueprintType)
struct FHSMConfig
{
	GENERATED_BODY()

public:

	FHSMConfig();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<TEnumAsByte<EEquipmentFlags>,TSubclassOf<UBaseState>> EquipmentFlagMap;
};


UCLASS(BlueprintType)
class UHSMConfiguration: public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FHSMConfig EquipmentFlagStruct;

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};



