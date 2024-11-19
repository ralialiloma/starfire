
#pragma once
#include "CoreMinimal.h"
#include "Sf_TetherPointTest.h"
#include "Starfire/Character_TP/Behaviour/BlackboardKeyHelperLibrary.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "UObject/Interface.h"
#include "Sf_TetherPointReceiver.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class USf_TetherPointReceiver : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	UTetherPoint* GetActiveTetherPoint() const;
	
protected:
	void RunUpdateLoop();
	TFuture<void> Update();
	TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> RunTests_Async(const TArray<TWeakObjectPtr<UTetherPoint>>& AllTetherPoints);
	UFUNCTION()
	bool NeedsUpdate();
	UFUNCTION()
	void UpdateTetherPoint(UTetherPoint* NewTetherPoint);
	TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> ImportTetherPoints();
#pragma endregion

#pragma region Properties
protected:
	UPROPERTY(Instanced, EditDefaultsOnly, Category = "EQS")
	TArray<USf_TetherPointTest*> Tests;
	UPROPERTY()
	TArray<TWeakObjectPtr<USf_TetherPointTest>> WeakTests;
	
	UPROPERTY(EditDefaultsOnly,Category = "EQS")
	float UpdateRateInSeconds = 1;
	UPROPERTY(EditDefaultsOnly,Category = "EQS")
	ELocationBlackboardKey LocationToUpdate;
	
	FTimerHandle UpdateProcess;
	UPROPERTY()
	UTetherPoint* ActiveTetherPoint;
#pragma endregion

};