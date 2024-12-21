#pragma once
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Sf_BreakerPillar.generated.h"

UCLASS(BlueprintType)
class ASf_BreakerPillar: public AActor
{
	GENERATED_BODY()
	
public:
	ASf_BreakerPillar();
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	USf_DamageController* GetDamageController() const;

	UFUNCTION(BlueprintCallable)
	bool IsActive() const;
	UFUNCTION(BlueprintCallable)
	bool SetRestore(float RestoreValue, float& OutRemainingValue);
	UFUNCTION(BlueprintCallable)
	void FullRestore();

protected:

	friend class ASf_BreakerCore;

	UFUNCTION(BlueprintNativeEvent)
	void OnBreak();
	UFUNCTION(BlueprintNativeEvent)
	void OnRestore();
	UFUNCTION(BlueprintNativeEvent)
	void OnRestoreProgress(float Alpha);

	UFUNCTION()
	void BreakPillar();

public:

	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnBreak_BP;
	FSf_VoidDelegate_CPP OnBreak_CPP;
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentRestoreValue = 0;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	float MaxRestoreValue = 100;

	//Components
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USf_DamageController* DamageController;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USf_Hitbox* PrimaryHitbox;
};
