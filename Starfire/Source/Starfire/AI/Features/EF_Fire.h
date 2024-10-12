#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "EF_Fire.generated.h"

DEFINE_LOG_CATEGORY_STATIC(EF_Fire, Display, Display);


UCLASS(BlueprintType,Blueprintable)
class STARFIRE_API UEF_Fire : public UEnemyFeature
{
	GENERATED_BODY()

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Fire")
	void StartFire(bool bScoped);
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Fire")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Fire")
	void StartReload();
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Fire")
	void StopReload();
private:
	void DoFire(EInputSignalType InputSignalType, bool bScoped, bool& bHitOtherNPC);
	bool OtherNPCWouldBeHit();
	static bool OtherNPCWouldBeHit(FHitResult HitResult);

#pragma endregion
	
#pragma region Properties
public:
	FSf_VoidDelegate_CPP OnReloadFinish_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnReloadFinish_BP;

	FSf_VoidDelegate_CPP OnReloadStopped_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnReloadStopped_BP;

	FSf_VoidDelegate_CPP OnFireStopped_CPP;
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnFireStopped_BP;


private:
	bool bIsFiring;
	int FiredBullets;
	FTimerHandle FireTimer;
#pragma endregion


};