#pragma once
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Sf_BreakerShard.generated.h"

UCLASS(BlueprintType)
class ASf_BreakerShard: public AActor
{
	GENERATED_BODY()
	
public:
	ASf_BreakerShard();
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintCallable)
	USf_DamageController* GetDamageController() const;

protected:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USf_DamageController* DamageController;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	USf_Hitbox* PrimaryHitbox;
};
