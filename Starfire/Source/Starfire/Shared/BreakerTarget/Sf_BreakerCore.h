#pragma once
#include "Sf_BreakerPillar.h"
#include "Starfire/Sf_Bases/Sf_Delegate.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"
#include "Sf_BreakerCore.generated.h"


class ASf_GameState;
DECLARE_LOG_CATEGORY_EXTERN(LogBreakerTarget, Log, All);

UCLASS(BlueprintType,Abstract,Blueprintable)
class ASf_BreakerCore: public AActor
{
	GENERATED_BODY()
public:
	
	ASf_BreakerCore(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void StartGame();
	
#pragma region Functions
public:

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	int GetNumPillars() const;
	UFUNCTION(BlueprintCallable)
	int GetNumActivePillars() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRawProgress() const;
	UFUNCTION(BlueprintCallable)
	float GetPillarProgress() const;

protected:

	UFUNCTION(BlueprintNativeEvent)
	void OnProgressChanged();
	UFUNCTION(BlueprintNativeEvent)
	void OnProgressFull();
	UFUNCTION(BlueprintNativeEvent)
	void OnProgressEmpty();
	
	void UpdateProgress(float Value);

	float GetDividedPillarProgress() const;
	int CalculateNumActivePillars() const;

	ASf_BreakerPillar* GetRandomInactivePillar();
	TArray<ASf_BreakerPillar*> GetInactivePillars();
	
	void ImportPillars();
	void RegisterPillar(ASf_BreakerPillar* PillarToRegister);
	void SyncPillars();
#pragma endregion
	
#pragma region Properties
public:
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnProgressChanged_BP;
	FSf_VoidDelegate_CPP OnProgressChanged_CPP;
	
	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnFullProgress_BP;
	FSf_VoidDelegate_CPP OnFullProgress_CPP;

	UPROPERTY(BlueprintAssignable)
	FSf_VoidDelegate_BP OnZeroProgress_BP;
	FSf_VoidDelegate_CPP OnZeroProgress_CPP;

protected:

	//Progress
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ProgressPerSecond = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxProgress = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Progress = 50.f;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ASf_BreakerPillar*> BreakerPillars {};

	UPROPERTY(BlueprintReadOnly)
	ASf_BreakerPillar* ChargingPillar = nullptr;
	
#pragma endregion
};