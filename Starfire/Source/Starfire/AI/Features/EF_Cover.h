#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "EF_Cover.generated.h"


UCLASS(Blueprintable)
class STARFIRE_API UEF_Cover : public UEnemyFeature
{
	GENERATED_BODY()

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	bool EnterCover();

	UFUNCTION(BlueprintCallable)
	bool ExitCover();
	
	UFUNCTION(BlueprintPure)
	bool CanBeHitByPlayer() const;

	UFUNCTION(BlueprintPure)
	bool IsInCoverState() const;

private:
	bool CanBeHitByPlayer(float HeightOffset) const;
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly)
	float MinCoverHeight = 40;

	UPROPERTY(EditDefaultsOnly)
	float MaxCrouchCoverHeight = 60 ;
private:
	UPROPERTY()
	bool bIsInCoverState = false;
#pragma endregion


};