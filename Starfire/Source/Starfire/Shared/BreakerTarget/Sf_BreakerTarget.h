﻿#pragma once
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Sf_BreakerTarget.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(LogBreakerTarget, Log, All);

UCLASS(BlueprintType,Abstract,Blueprintable)
class ASf_BreakerTarget: public AActor
{
	GENERATED_BODY()
public:
	ASf_BreakerTarget(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	float GetProgress() const;
	UFUNCTION(BlueprintCallable)
	USf_DamageController* GetDamageController() const;
private:
	void DoProgress(float DeltaSeconds) const;
	void OnDamageReceived(float ReceivedDamage);
#pragma endregion
#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnProgressChanged_BP;
	FSf_VoidDelegate_CPP OnProgressChanged_CPP;
	
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnFullProgress_BP;
	FSf_VoidDelegate_CPP OnFullProgress_CPP;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float ProgressionRatePerSecond = 2;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float MaxProgression = 1;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_DamageController* SfDamageController;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	USf_Hitbox* PrimaryHitbox;
private:
	FDelegateHandle DelHandle;
#pragma endregion
};