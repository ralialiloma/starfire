#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "EF_Fire.generated.h"


UCLASS(Blueprintable)
class STARFIRE_API UEF_Fire : public UEnemyFeature
{
	GENERATED_BODY()

#pragma region Functions
public:
	void StartFire(bool bScoped);
	void StopFire();
private:
	void DoFire(EInputSignalType InputSignalType, bool bScoped);
	bool OtherNPCWouldBeHit();

#pragma endregion
	
#pragma region Properties
protected:
	float FireRate;
private:
	bool bIsFiring;
	int FiredBullets;
	FTimerHandle FireTimer;
#pragma endregion


};