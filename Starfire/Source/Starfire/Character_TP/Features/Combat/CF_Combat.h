#pragma once
#include "CoreMinimal.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_Combat.generated.h"

DEFINE_LOG_CATEGORY_STATIC(EF_Combat, Display, Display);

#pragma region Structs And Enums
UENUM(BlueprintType)
enum class EStopFireReason: uint8
{
	None = 0,
	FireBlock = 1,
	ManualInput = 2,
	HitOtherNPC = 3,
	StartedReload = 4,
	CannotHitPlayer = 5,
};

USTRUCT(BlueprintType)
struct FStopFireInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(BlueprintReadWrite)
	EStopFireReason StopFireReason;

	UPROPERTY(BlueprintReadWrite)
	EFireBlock FireBlock;

	FStopFireInfo();

	explicit FStopFireInfo(EStopFireReason InStopFireReason, EFireBlock InFireBlock = EFireBlock::None);

	FString ToString();
};
#pragma endregion

UCLASS()
class STARFIRE_API UCF_Combat : public USf_CharacterFeature
{
	GENERATED_BODY()

#pragma region Functions
public:

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	bool StartFire(int MaxMissedBulletsBeforeStop, bool bInClearFocusAfterFiring = true);

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	bool InstantReload();
	
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	void StopReload();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	void DoIllegalReload();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Combat")
	bool Melee();

	UFUNCTION(BlueprintPure, Category = "EnemyFeature|Combat")
	FStopFireInfo GetLastStopFireInfo();

protected:

private:
	void StopFire(FStopFireInfo StopFireInfo);
	void DoFire(const EInputSignalType InputSignalType, int MissedBulletsBeforeStop);
	bool OtherNPCWouldBeHit();
	static bool WouldHitPlayer(const FHitResult& HitResult);
	bool WouldHitNPC(const FHitResult& HitResult);

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

	FDelegateHandle OnReloadFinishHandle = FDelegateHandle();
	FDelegateHandle OnReloadStoppedHandle = FDelegateHandle();

private:
	int MissedBullets = 0;
	FStopFireInfo LastFireStopInfo;
	FTimerHandle FireHandle;
	bool bIsFiring;
	int FiredBullets;
	bool bCLearFocusAfterFiring;
#pragma endregion


};