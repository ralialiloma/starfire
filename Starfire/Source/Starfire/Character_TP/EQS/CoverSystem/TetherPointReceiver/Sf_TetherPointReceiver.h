
#pragma once
#include "CoreMinimal.h"
#include "Sf_TetherPointTest.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"
#include "UObject/Interface.h"
#include "Sf_TetherPointReceiver.generated.h"
UCLASS()
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
	UFUNCTION()
	void Update();
	UFUNCTION()
	TArray<UTetherPoint*> RunTests(const TArray<UTetherPoint*>& AllTetherPoints);
	UFUNCTION()
	bool NeedsUpdate();
#pragma endregion

#pragma region Properties
protected:
	UPROPERTY(Instanced, EditDefaultsOnly)
	TArray<USf_TetherPointTest*> Tests;
	UPROPERTY(EditDefaultsOnly)
	float UpdateRateInSeconds = 1;
	
	FTimerHandle UpdateProcess;
	UPROPERTY()
	UTetherPoint* ActiveTetherPoint;
#pragma endregion

};