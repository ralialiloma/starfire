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
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool EnterCover();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool ExitCover();
	
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool CanBeHitByPlayer() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
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